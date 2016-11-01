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

/************************************************************************/
/* DEFINITIONS                                                          */
/************************************************************************/
extern uint8_t TakeSample;
/************************************************************************/
/* VARIABLES                                                            */
/************************************************************************/
volatile uint8_t transmitflag = 0;

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
int main(void)
{
	
	/************************************************************************/
	// Set Pin C2 as output for testing purposes.
	DDRC   |= (1<<PORTC2);	// Set Port C2 as output.
	
	timer1_init();		// Set up Timer 1.
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
	receiver_setenabled(1);	// Activate the receiver.
	
	/************************************************************************/
    while(1)
    {
		
		/* Transmitter is disabled for this experiment, we only need to turn a led on and off once.
		if(transmitflag) {
			transmitter_timertick();	// Trigger the transmitter.				
			transmitflag--;				// Remove one ticket from the flag.
		}*/
		
		// Sample.
		if(TakeSample){
			TakeSample = 0;
			receiver_sample();
			if(TakeSample){
				/*we cant handle current receiver speed :'( */
			}
		}

		
		// Process new data from the UART.
		if(uart_char_waiting()) {
//			transmitter_add(uart_read());	// Simply pass through data.
			uartcontroller_process(uart_read());
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

}