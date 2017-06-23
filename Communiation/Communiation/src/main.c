/********************
 * Specs:
 *
 * Dark Sense
 *		ADC Burst mode (250 Khz) for 50 samples (1/(250*10^3)*50 = 0.2 ms, where the light is only one for approx 0.1 ms)
 *		Filters to make the data interpretable
 *		Transmission of data via uart
 *		BONUS: Run algorithm on platform
 * 
 * Superblink
 *		Port Superblink
 *      Create a sync between super blink and the burst mode
 * 	 ?  Build RX receiver to interpret super blink commands.
 * 
 * Communication
 *		Slow continuous ADC mode (100 Khz)
 *		Invent some kind of pulse detection
 *		Modify superblink to be able to transmit data.
 *
 *
 * Not filled means TODO
 * o Means Work in progress
 * ? means done but needs to be tested
 * X = Completed and tested
 *************************************************/

#include <asf.h>
#include <interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "FIFO.h"
#include "uart.h"
#include "sense.h"

/*
 * 41 Mhz gives a nice round prescale value of 84/6 = 14. We want a ADC frequency of 250Khz
 * So there are 6000/250 = 24 cycles in a single ADC conversion.
 */

void timer_setup(void){
	pmc_enable_periph_clk(ID_TC0);
		
	TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK4 | TC_CMR_CPCTRG| TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC; // set clock to use Mclk/2 and reset on compare match
	TC0->TC_CHANNEL[0].TC_RC = 650000; //Timer reset value
	TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS; //enable interrupt on campare match
	TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG; // enable and start TC0
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init(); // this seems to set all internal clocks
	board_init();  // This seems to set all pins to their correct types for the arduino due
	
	sense_setup();
	uart_setup();
	timer_setup();
	
	irq_initialize_vectors();
	NVIC_EnableIRQ(UART_IRQn);
	NVIC_EnableIRQ(ADC_IRQn);
	NVIC_EnableIRQ(TC0_IRQn);	
	
	uart_send_string("Starting...\n");
	while(1){
		
	}
}


void TC0_Handler(void){
	 /* this is required to clear the interrupt flag. Not having this line results in infinite timer interrupts*/
	uint32_t status = TC0->TC_CHANNEL[0].TC_SR;
	uart_send_char('\n');
	
	/*Start ADC*/
	ADC->ADC_CR = ADC_CR_START;
}


