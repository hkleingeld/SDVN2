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
#include "features/Features.h"
#include <string.h>

/************************************************************************/
/* DEFINITIONS                                                          */
/************************************************************************/


/************************************************************************/
/* VARIABLES                                                            */
/************************************************************************/

volatile uint8_t transmitflag = 0;

volatile uint8_t kick = 0;
uint16_t sampledData[800] = {0};
void * sendCtr;
extern volatile uint8_t ADC_select;

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
int main(void)
{
	uint8_t adc = 0;
	int i = 0;
	uint8_t escapechar = 0;

	/*variables for "real time" analysis*/
	uint16_t min = 0;
	uint16_t max = 0;
	uint16_t diff = 0;
	uint16_t daff = 0;
	/************************************************************************/
	// Set Pin C2 as output for testing purposes.
	//DDRC   |= (1<<PORTC2);	// Set Port C2 as output.
	
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
// 		if(uart_char_waiting()) {
// 			//			transmitter_add(uart_read());	// Simply pass through data.
// 			uartcontroller_process(uart_read());
// 		}

		switch(adc){
			case 1: sampledData[i] = spi_adc_read(ADC_CHANNEL1); break;
			case 2: sampledData[i] = spi_adc_read(ADC_CHANNEL2); break;
			case 3: sampledData[i] = spi_adc_read(ADC_CHANNEL3); break;
			case 4: sampledData[i] = spi_adc_read(ADC_CHANNEL4); break;
			default: break;
		}

		i++;
		
		if(i == 800){
			/*transmit data*/				
			escapechar = 0;
			while(escapechar != 2){
				if(uart_writebuffer_ready()){
					uart_write(0xff);
					escapechar++;
				}
			}
			sendCtr = sampledData;
			while(sendCtr != &sampledData[800]){
				if(uart_writebuffer_ready()){
					uart_write(*(uint8_t*) sendCtr);
					sendCtr++;
				}	
			}

			while(UCSR0B & (1 << UDRIE0)); /*wait until  data ready interrupt is turned off (aka, we are done sending data)*/
			
			/*Real time analysis*/
			min = Minimum(800, sampledData);
			max = Maximum(800, sampledData);
			diff = max - min;
			/*Set real time monitoring*/
			diff = diff >> 4;
			//spi_gpio_write(GPIO_BANK_LED16_9, (uint8_t) (diff & 0x00FF));
			//uart_write((uint8_t) (diff & 0x00FF));
			/*End of real time analysis*/
			
			daff = PulseLenght(800, 10, sampledData);
			//spi_gpio_write(GPIO_BANK_LED8_1, (uint8_t) (daff & 0x00FF));
			//uart_write((uint8_t) (daff & 0x00FF));
			
			switch(ADC_select){
				case 1: adc = 1; break;
				case 2: adc = 2; break;
				case 3: adc = 3; break;
				case 4: adc = 4; break;
				default: adc = 0; break;
			}			
			_delay_ms(1000); /*extra time for syncing, solved a werid bug*/
			
			while((PINC & 0x04) == 0x04); /*Wait until sync pin is low*/
			while((PINC & 0x04) == 0x00); /*Wait until sync pin is High*/
			i = 0;
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