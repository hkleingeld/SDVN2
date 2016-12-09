/*
 * SuperBlink.c
 *
 * Created: 15-11-2016 13:54:24
 * Author : Hajo
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "uart/uart.h"
#include "uart/UartController.h"

extern uint16_t LedTargetTime;
extern uint16_t PeriodTime;
extern uint16_t SyncTime;
extern uint8_t FetNr;

void timer1_init(void) {
	
	// Collected these commands in one assignment.
	// TCCR1B |= (1<<WGM12);	// Configure timer 1 for CTC mode.
	//
	// Clock scaler selection.
	//	TCCR1B |= (1<<CS10);				// Start timer at Fcpu/1.
	//	TCCR1B |= (1<<CS11);				// Start timer at Fcpu/8.
	//	TCCR1B |= ((1<<CS11) | (1<< CS10));	// Start timer at Fcpu/64.
	//	TCCR1B |= (1<<CS12);				// Start timer at Fcpu/256.
	//	TCCR1B |= ((1<<CS12) | (1<< CS10));	// Start timer at Fcpu/1024.
	TCCR1B = 0b00001001; /*prescaler of 1*/
	
	TIMSK1 = 0b00000010;	// TIMSK1 |= (1<<OCIE1A);	// Enable CTC interrupt.
	
	// Counter value selection.
	// OCR1A = Fclk / (clock_divider * required_interrupt_frequency).
	OCR1A =	180;	// Set CTC compare value to 100000 Hz at 16Mhz.
}


int main(void)
{
	uart_init();		// Set up UART.
	transmitter_init();
	timer1_init();
	sei();				// Enable global interrupts.
	
	DDRC |= 0xff;
	
	uart_write('O');	// Test UART.
	uart_write('K');
	uart_write('I');
	

	/************************************************************************/

	while(1){
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
	static uint16_t LoopCtr = 0;

	if(LoopCtr > PeriodTime){
		PORTC |= FetNr;
		LoopCtr = 0;
	}

	if(LoopCtr == LedTargetTime){
		PORTC = 0x00;
	}
	
	if(LoopCtr == SyncTime){
		PORTC |= 0x20;
	}
	

	LoopCtr++;
}
