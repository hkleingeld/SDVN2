/*
 * uart.c
 *
 * Created: 22-6-2017 14:44:54
 *  Author: Hajo
 */ 

#include "FIFO.h"
#include "SuperBlink.h"
#include <asf.h>
#include <interrupt.h>

static FIFO * uart_fifo_TX;
static FIFO * uart_fifo_RX;
static uint8_t uart_sending = 0;

typedef enum {IDLE, BUSY , ESCAPE} TransmissionState;

/*special transmit characters*/
#define ESC   0xd0
#define START 0xc0
#define STOP  0xc0

/*Message Identifiers*/
#define BLINK_COMMAND 0
#define COMMUNICATION_COMMAND 1
#define SENSE_COMMAND 2

static void uart_start_tx(void){
	if(!uart_sending){
		/*enable TX interupt, so interrupt gets called when this transmission is called.*/
		UART->UART_IER = UART_IER_TXRDY;
		
		/*Send the next sendable byte*/
		char val_to_send;
		FIFOPull(uart_fifo_TX,&val_to_send);
		UART->UART_THR = val_to_send;
		
		uart_sending = 1;
	}
}

uint8_t uart_send_char(char new_char){
	uint8_t retval = FIFOPush(uart_fifo_TX, new_char);
	
	uart_start_tx();
	
	return retval;
}

uint8_t uart_send_string(const char * string){
	while(*string != '\0'){
		uint8_t result = FIFOPush(uart_fifo_TX, *string);
		if(result != FIFO_OK){
			return(result);
		}
		string++;
	}
	
	uart_start_tx();
	
	return(FIFO_OK);
}

void uart_setup(void){
	pmc_enable_periph_clk(ID_UART);
	
	UART->UART_MR = UART_MR_PAR_NO | UART_MR_CHMODE_NORMAL;	//no parity, no fancy test setups.
	UART->UART_IER = UART_IER_RXRDY;			            // enable rx
	UART->UART_BRGR = 46;								    // set boudrate to 115200 boud
	UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN;			// enable rx and tx

	uart_fifo_TX = FIFOInit(200);
	uart_fifo_RX = FIFOInit(200);
}



static void uart_interpret_message(uint8_t * msg, uint8_t size){
	/* Possible messages:
	 * Blink command:
	 * ['0'] [Period MSB] [Period LSB] [T-on MSB] [T-on LSB] [Light power]
	 * 
	 * Communication command:
	 * ['1'] [NR of Bytes to send] [Byte 0] [Byte 1] .. [Byte n-1]
	 *
	 * Sense command:
	 * ['2'] [Photodiode to use]
	 */
	
	if(size == 0) return;
	
	switch(msg[0]){
		case BLINK_COMMAND:
			if(size != 6) return;
			superblink_set_Period((msg[1] << 8) | msg[2]);
			superblink_set_ton((msg[3] << 8) | msg[4]);
			superblink_set_LedNr(msg[5]);
			break;
		case COMMUNICATION_COMMAND:
			//TODO, not enought info yet
			break;
		case SENSE_COMMAND:
			//TODO, not enought info yet
			break;
		default:
		break;
		
		return;
	}
}
	

void uart_handle_rx(void){
	static uint8_t ReceiveBuffer[20] = {0};
	static uint8_t index = 0;
	static TransmissionState state = IDLE;
	
	uint8_t newData;
	while(FIFOPull(uart_fifo_RX,&newData) == FIFO_OK){
		switch(state){
			case IDLE:
				index = 0;
				if(newData == START){
					state = BUSY;
				}
				break;
				
			case BUSY:
				if(newData == ESC){
					state = ESCAPE;
					break;
				}
				if(newData == STOP){
					 uart_interpret_message(ReceiveBuffer,index);
					 break;
				}
				ReceiveBuffer[index] = index;
				index++;
				break;
				
			case ESCAPE:
				ReceiveBuffer[index] = index;
				index++;
				state = BUSY;
				break;
				
			default:
			state = IDLE;
			break;
		}
	}
}

void UART_Handler(void){
	/*Data has been transmit, and the send register is empty*/
	if(UART->UART_SR & UART_SR_TXRDY){
		uint8_t val_to_send = 0;
		uint8_t res = FIFOPull(uart_fifo_TX,&val_to_send);
		
		/*there was data in the FIFO, send the data*/
		if(res == FIFO_OK){
			UART->UART_THR = val_to_send;
		}
		else{
			/*no data in the FIFO, stop sending data*/
			uart_sending = 0;
			UART->UART_IDR = UART_IDR_TXRDY;
		}
	}
	
	/*byte received, and its in the receive register*/
	if(UART->UART_SR & UART_SR_RXRDY){
		FIFOPush(uart_fifo_RX,UART->UART_RHR);
	}
}