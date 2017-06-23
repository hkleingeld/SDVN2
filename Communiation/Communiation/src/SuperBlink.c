/*
 * SuperBlink.c
 *
 * Created: 22-6-2017 16:32:50
 *  Author: Hajo
 */ 

#include "SuperBlink.h"
#include "sense.h"
#include <interrupt.h>
#include <asf.h>

uint32_t TonTime = 500*420;
uint32_t Period = 1000*420;
uint8_t LedNr = 1;

void setup_superblink(void){
	pmc_enable_periph_clk(ID_TC1);
		
	TC1->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1 | TC_CMR_CPCTRG| TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | // set clock to use Mclk/2 and reset on compare match
								TC_CMR_ACPA_SET | TC_CMR_ACPC_CLEAR; // Set TIOA pin if timer == RC, Clear TIOA pin if timer == RA.
								
	TC1->TC_CHANNEL[0].TC_RA = TonTime;//Turn off time
	TC1->TC_CHANNEL[0].TC_RB = Period - 420;//Start time for burst mode (10 us before the light turns on)
	TC1->TC_CHANNEL[0].TC_RC = Period; //Period time (also rimer reset)
	
	TC1->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG; // enable and start TC1
}

void superblink_set_ton(uint16_t val){
	TonTime = val*420;
}

void superblink_set_Period(uint16_t val){
	Period = val*420;
}

void superblink_set_LedNr(uint8_t val){
	LedNr = val;
}

void TC1_Handler(void){
	/* this is required to clear the interrupt flag. Not having this line results in infinite timer interrupts*/
	uint32_t status = TC0->TC_CHANNEL[1].TC_SR;
	sense_start_burst_mode();
}