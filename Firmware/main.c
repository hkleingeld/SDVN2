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


/************************************************************************/
/* VARIABLES                                                            */
/************************************************************************/

volatile uint8_t transmitflag = 0;

volatile uint8_t kick = 0;
uint16_t sampledData[800] = {0};
void * sendCtr;

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
int main(void)
{
	int i = 0;
	uint8_t escapechar = 0;
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
	
		sampledData[i] = spi_adc_read(ADC_CHANNEL2);
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