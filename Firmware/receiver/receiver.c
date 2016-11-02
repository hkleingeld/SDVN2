/*
 * receiver.c
 *
 * Created: 25-11-2014 11:34:22
 * Original Author: Lennart Klaver
 * Edited By: Hajo Kleingeld
 */ 
/************************************************************************/
/* LIBRARIES                                                            */
/************************************************************************/
#include "receiver.h"
#include <string.h>

/************************************************************************/
/* DEFINITIONS                                                          */
/************************************************************************/
	typedef enum {CHANNEL1, CHANNEL2, CHANNEL3, CHANNEL4, NUM_CHANNELS} channel_t;
		
/************************************************************************/
/* PROTOTYPES                                                           */
/************************************************************************/
void process_sample(uint16_t sample, channel_t channel);

/************************************************************************/
/* VARIABLES                                                            */
/************************************************************************/
volatile uint16_t SYSTEMTIME = 0;

uint8_t TakeSample = 0;
volatile uint8_t receiverenabled = 0;
uint8_t sampledData[1500] = {0}; /* Data storage for samples, can hold 300 samples of 4 channels with 1 byte saved as separator*/
uint16_t sampleDataFinger = 0;   /* At what place are we currently string sample data*/




/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
void receiver_init(void) {
	timer0_init();
}

/**
 * Do a channel measurement.
 */
void receiver_measure() {
}

void receiver_sample(void) {
	uint16_t sample1, sample2, sample3, sample4;
	if(receiverenabled){
		sample1 = spi_adc_read(ADC_CHANNEL2);
		sample2 = spi_adc_read(ADC_CHANNEL3);
		sample3 = spi_adc_read(ADC_CHANNEL4);
		sample4 = spi_adc_read(ADC_CHANNEL1);
		
		sampledData[sampleDataFinger] = ((sample1 >> 4) & 0x00FF);
		sampledData[sampleDataFinger +1] = ((sample2 >> 4) & 0x00FF);
		sampledData[sampleDataFinger +2] = ((sample3 >> 4) & 0x00FF);
		sampledData[sampleDataFinger +3] = ((sample4 >> 4) & 0x00FF);
		sampledData[sampleDataFinger +4] = '\r';
		
		sampleDataFinger += 5;
		
		if(sampleDataFinger == 1500){
			disable_timer1();
			uart_write('D');
			receiverenabled = 0;
			sampleDataFinger = 0;
		}
	}
}

/**
 * Handle the ADC samples and decode them.
 */
void process_sample(uint16_t sample, channel_t channel) {	

}

void receiver_reset(void) {
	memset(&sampledData[0],0,sizeof(sampledData));
	sampleDataFinger = 0;
}

void receiver_setenabled(uint8_t enabled) {
	receiverenabled = enabled;
}


/**
 * ISR for overflow interrupt of Timer 0 (8-bit timer).
 */
ISR(TIMER0_OVF_vect) {
	SYSTEMTIME++;	// Update the system counter.
	

	// Enable the lines below to measure the time between two ticks on port C2.
	//PORTC |= (1<<PORTC2);	// Switch on.
	//PORTC &= ~(1<<PORTC2);	// Switch off.
}