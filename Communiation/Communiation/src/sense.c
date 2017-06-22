/*
 * sense.c
 *
 * Created: 22-6-2017 14:50:56
 *  Author: Hajo
 */ 
#define ADC_CLOCK 6000000 

#include "sense.h"
#include <asf.h>
#include <interrupt.h>

void sense_setup(void)
{
	pmc_enable_periph_clk(ID_ADC);
	
	adc_init(ADC, sysclk_get_main_hz(), ADC_CLOCK, 8);		//6Mhz gives a nice prescale value of 14 (consistent behaviour)
	adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1);
	adc_set_resolution(ADC, ADC_MR_LOWRES_BITS_12);			//10 bit mode
	adc_enable_channel(ADC, ADC_CHANNEL_5);					//Use channel 5 (somehow this is A2....)
	adc_enable_interrupt(ADC, ADC_IER_DRDY);				//trigger ADC_IrqHandler on data ready
	adc_configure_trigger(ADC, ADC_TRIG_SW, 0);				//Start ADC conversions on Software trigger only.
	
	//adc_configure_trigger(ADC, ADC_TRIG_TIO_CH_2, 0);		//Start ADC conversions on Timer II (TODO).
}

void sense_start_burst_mode(void){
	/*configure timer*/
	
	/*configure ADC for burst*/
	
	/*sync*/
	
}



void ADC_Handler(void)
{
	/*this needs to be read here, or the interrupt flag is not cleared*/
	uint32_t res = ADC->ADC_LCDR;
// 	itoa(res,itoabuffer,10);
// 	uart_send_string(itoabuffer);
}