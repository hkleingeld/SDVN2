/*
 * Main.c
 *
 * Created: 19-11-2014 9:52:57
 *  Author: Lennart Klaver
 */ 
/************************************************************************/
/* LIBRARIES                                                            */
/************************************************************************/
#include "main.h"
#include <string.h>

/************************************************************************/
/* DEFINITIONS                                                          */
/************************************************************************/
#define IDLE			0
#define START			1
#define LED_ON			2
#define PD_SAMPLING		3
#define LED_OFF			4
#define TRANSMIT		5	

/************************************************************************/
/* VARIABLES                                                            */
/************************************************************************/
extern uint8_t TakeSample;
volatile uint8_t transmitflag = 0;
extern volatile uint8_t receiverenabled;
extern volatile uint16_t LedTargetTime;

volatile uint8_t kick = 1;

extern uint8_t LapsToGo;

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
int main(void)
{
	uint16_t sampledData[800] = {0}; /* Data storage for samples, can hold 300 samples of 4 channels with 1 byte saved as separator*/
	uint8_t CurrentState = IDLE;
	uint8_t NextState = IDLE;
	uint16_t NumOfSamples = 0;
	uint8_t escapechar = 0;
	void* sendCtr;
	/************************************************************************/
	// Set Pin C2 as output for testing purposes.
	DDRC   |= (1<<PORTC2);	// Set Port C2 as output.
	
	//timer1_init();		// Set up Timer 1.
	spi_init();			// Set up SPI.
	spi_gpio_init();	// Set up GPIO.
	uart_init();		// Set up UART.
	
	transmitter_init();	// Set up the transmitter.
	receiver_init();	// Set up the receiver.
	
	spi_gpio_test();	// Test GPIO.
		
	sei();	// Enable global interrupts.
	
	//TODO: full self test.
	
	uart_write('O');	// Test UART.
	uart_write('K');
	/************************************************************************/
	
	
	receiver_measure();		// Do a medium measurement.
	receiver_reset();		// Reset the receiver.
	//receiver_setenabled(1);	// Activate the receiver.
	
	/************************************************************************/
	
	while(1){
		if(kick == 0){
			continue;
		}
		
		if(--kick){
			uart_write('X'); /*we are too slow :'(*/
		}
		
		CurrentState = NextState;
		
		switch(CurrentState){
		case IDLE:
			kick++;
			if(uart_char_waiting()) {
				//			transmitter_add(uart_read());	// Simply pass through data.
				uartcontroller_process(uart_read());
			}
			if(LedTargetTime != 0){
				NextState = START;
			}
			break;
		case START:
			memset(&sampledData[0],0,sizeof(sampledData));
			NumOfSamples = 0;
			NextState = PD_SAMPLING;
			timer1_init(); /*starts timer and sets led target time*/
			break;
			
		case LED_ON:
			spi_gpio_write(GPIO_BANK_LED8_1,	0x01);/*turn led on*/
			NextState = PD_SAMPLING;
			break;
			
		case PD_SAMPLING:
			sampledData[NumOfSamples] = spi_adc_read(ADC_CHANNEL2);
			if(++NumOfSamples == 800){
				NextState = TRANSMIT;
			}
			
			if(NumOfSamples == 5){
				NextState = LED_ON;
			}

			if(NumOfSamples == LedTargetTime){
				NextState = LED_OFF;
			}
			break;
			
		case LED_OFF:
			spi_gpio_write(GPIO_BANK_LED8_1,	0x00); /*turn led off*/
			NextState = PD_SAMPLING;
			break;

		case TRANSMIT:
			disable_timer1();
			sendCtr = &sampledData[0];
			
			escapechar = 0;
			while(escapechar != 2){
				if(uart_writebuffer_ready()){
					uart_write(0xff);
					escapechar++;
				}
			}
			
			while(sendCtr != &sampledData[800]){
				if(uart_writebuffer_ready()){
					uart_write(*(uint8_t*) sendCtr);
					sendCtr++;
				}
			}
			NextState = IDLE;
			if(LapsToGo != 0){
				LapsToGo --;
			}
			else{
				LedTargetTime = 0;
			}
			
			kick++;
			break;
		}
	}
	/************************************************************************/
}

/************************************************************************/
/* INTERRUPT SERVICE ROUTINES                                           */
/************************************************************************/
/**
 * ISR for compare interrupt of Timer 1 (16-bit timer).
 */
ISR(TIMER1_COMPA_vect) {
	kick++;
}