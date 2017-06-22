/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "FIFO.h"

FIFO * adc_fifo;
FIFO * uart_fifo_TX;
FIFO * uart_fifo_RX;
uint8_t uart_sending = 0;
uint8_t adc_working = 1;

char itoabuffer[20];

/*
 * 41 Mhz gives a nice round prescale value of 84/6 = 14. We want a ADC frequency of 250Khz
 * So there are 6000/250 = 24 cycles in a single ADC conversion.
 */

void uart_start_tx(void){
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

#define ADC_CLOCK 6000000 

void adc_setup(void)
{
	pmc_enable_periph_clk(ID_ADC);
	
 	adc_init(ADC, sysclk_get_main_hz(), ADC_CLOCK, 8);		//6Mhz gives a nice prescale value of 14 (consistent behaviour)
 	adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1);
 	adc_set_resolution(ADC, ADC_MR_LOWRES_BITS_12);			//10 bit mode
 	adc_enable_channel(ADC, ADC_CHANNEL_5);					//Use channel 5 (somehow this is A2....)
 	adc_enable_interrupt(ADC, ADC_IER_DRDY);				//trigger ADC_IrqHandler on data ready
 	adc_configure_trigger(ADC, ADC_TRIG_SW, 0);				//Start ADC conversions on Software trigger only.
	
	//adc_configure_trigger(ADC, ADC_TRIG_TIO_CH_2, 0);		//Start ADC conversions on Timer II (TODO).
	
// 	ADC->ADC_MR = ADC_MR_FWUP | ADC_MR_PRESCAL(1) | ADC_MR_STARTUP_SUT16 | ADC_MR_SETTLING_AST9 | ADC_MR_TRACKTIM(9) | ADC_MR_TRANSFER(9);
// 	ADC->ADC_CHER = ADC_CHER_CH0;
// 	ADC->ADC_IER = ADC_IER_DRDY;
	
	adc_fifo = FIFOInit(1000);
}

void uart_setup(void){
	pmc_enable_periph_clk(ID_UART);
	
	UART->UART_MR = UART_MR_PAR_NO | UART_MR_CHMODE_NORMAL;	//no parity, no fancy test setups.	
	//UART->UART_IER = UART_IER_TXEMPTY;
	UART->UART_IER = UART_IER_RXRDY;			            // enable rx
	UART->UART_BRGR = 46;								    // set boudrate to 115200 boud
	UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN;;			// enable rx and tx

	uart_fifo_TX = FIFOInit(200);
	uart_fifo_RX = FIFOInit(200);
}

void timer_setup(void){
	pmc_enable_periph_clk(ID_TC0);
		
	TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK4 | TC_CMR_CPCTRG| TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC; // set clock to use Mclk/2 and reset on compare match
	TC0->TC_CHANNEL[0].TC_RC = 650000;
	TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS; //enable interrupt on campare match
	TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG; // enable and start TC0
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init(); // this seems to set all internal clocks
	board_init();  // This seems to set all pins to their correct types for the arduino due
	
	
	adc_setup();
	uart_setup();
	timer_setup();
	
	irq_initialize_vectors();
	NVIC_EnableIRQ(UART_IRQn);
	NVIC_EnableIRQ(ADC_IRQn);
	NVIC_EnableIRQ(TC0_IRQn);
	
	
	uart_send_string("Starting...\n");
	while(1){
		
	}
	
	/* Insert application code here, after the board has been initialized. */
}


void ADC_Handler(void)
{
	// Check the ADC conversion status
// 	if ((adc_get_status(ADC) & ADC_ISR_DRDY) == ADC_ISR_DRDY)
// 	{
// 		// Get latest digital data value from ADC and can be used by application
// 		uint32_t result = adc_get_latest_value(ADC);
// 		uint8_t fifo_status = FIFOPush(adc_fifo, result & 0x000000FF);
// 		fifo_status |= FIFOPush(adc_fifo, result & 0x0000FF00);
// 		
// 		if(fifo_status != FIFO_OK){
// 			
// 			TC0->TC_CHANNEL[0].TC_CCR |= TC_CCR_SWTRG;
// 			adc_working = 0;
// 		}
// 	}
// 
	uint32_t res = ADC->ADC_LCDR;
	itoa(res,itoabuffer,10);
	uart_send_string(itoabuffer);
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

void TC0_Handler(void){
	 /* this is required to clear the interrupt flag. Not having this line results in infinite timer interrupts*/
	uint32_t status = TC0->TC_CHANNEL[0].TC_SR;
	uart_send_char('\n');
	
	/*Start ADC*/
	ADC->ADC_CR = ADC_CR_START;
}


