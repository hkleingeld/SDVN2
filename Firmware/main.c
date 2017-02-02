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
#include "filter/filter.h"
#include <limits.h>
#include <string.h>

/************************************************************************/
/* DEFINITIONS                                                          */
/************************************************************************/
#define NR_OF_SAMPLES 40
#define DEFAULT_REFERENCE 0
#define NOISE_REFERENCE 1
#define NEW_SAMPLE_SET 2

#define MAX_NOISE_THRESHOLD 100
 


/************************************************************************/
/* VARIABLES                                                            */
/************************************************************************/
typedef struct Pulse{
	uint16_t samples[NR_OF_SAMPLES];
	uint16_t max;
	uint16_t min;
	uint16_t avg;
} Pulse;

volatile uint8_t transmitflag = 0;

volatile uint8_t kick = 0;

Pulse Measurments[3];
Pulse * referencePulse;
Pulse * currentPulse;
	
	
void * sendCtr;
extern volatile uint8_t ADC_select;

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
int main(void)
{
	uint8_t adc = ADC_select;
	int i = 0;
	uint8_t escapechar = 0;
	uint8_t darkSample = 0;
	uint8_t refSample = 0;
	uint8_t normSample = 0;
	uint32_t detect = 0;
	
	uint16_t maxmax = 0;
	uint16_t maxmin = 5000;
	
	uint32_t detectionThreshold = INT_MAX;

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
	/************************************************************************/
	
	// for the first time, create default reference
	currentPulse = &Measurments[NOISE_REFERENCE];
	darkSample = 1;
	
	while((PINC & 0x04) == 0x00); /*Wait until sync pin is High*/
	while((PINC & 0x04) == 0x04); /*Wait until sync pin is low*/
	_delay_ms(1);

	while(1){
		switch(adc){
			case 1: currentPulse->samples[i] = spi_adc_read(ADC_CHANNEL1); break;
			case 2: currentPulse->samples[i] = spi_adc_read(ADC_CHANNEL2); break;
			case 3: currentPulse->samples[i] = spi_adc_read(ADC_CHANNEL3); break;
			case 4: currentPulse->samples[i] = spi_adc_read(ADC_CHANNEL4); break;
			default: break;
		}
		i++;
		
		if(i == NR_OF_SAMPLES){
			
			//Always filter, and get maximum
			if(darkSample){
				uint16_t min = Minimum(NR_OF_SAMPLES, currentPulse->samples);
				uint16_t max = Maximum(NR_OF_SAMPLES, currentPulse->samples);
				
				if((max - min) > MAX_NOISE_THRESHOLD){
					//Above noise threshold? Retry and hope for less noise next loop
					darkSample = 1;
					uart_write('a');
				}
				else{
					//Continue
					uart_write('b');
					darkSample = 0;
					refSample = 100;
					//filter_CalculateStartupValues(currentPulse->samples);
					filter100_IRR(currentPulse->samples);
					currentPulse->avg = Average(NR_OF_SAMPLES, currentPulse->samples);
					currentPulse->min = Minimum(NR_OF_SAMPLES, currentPulse->samples);
					currentPulse->max = Maximum(NR_OF_SAMPLES, currentPulse->samples);
				}
			}
			else{
				filter100_IRR(currentPulse->samples);	
			}
			
			currentPulse->max = Maximum(NR_OF_SAMPLES, currentPulse->samples);
			
			if(normSample){ //normal sample, after first loop
				//uint32_t Absdiff = AbsoluteDifference(NR_OF_SAMPLES, currentPulse->samples, referencePulse->samples);
				
// 				uart_write((uint8_t) ((Absdiff & 0xFF000000) >> 24));
// 				uart_write((uint8_t) ((Absdiff & 0x00FF0000) >> 16));
// 				uart_write((uint8_t) (currentPulse->max &  0x000000FF));
//  			uart_write((uint8_t) ((currentPulse->max & 0x0000FF00) >> 8));
 				

// 				uart_write((uint8_t) ((currentPulse->max & 0xFF00) >> 8));
// 				uart_write((uint8_t) (currentPulse->max & 0xFF));
							
				if((currentPulse->max > (maxmax+1)) || (currentPulse->max < (maxmin-1))){
					detect++;
					if(detect > 3){
						uart_write('%');
					}
					else{
						uart_write('?');
					}
				}
				else{
					detect = 0;
					uart_write('-');
				}
			}
			
			if(refSample){ //make reference sample after this loop
				if(currentPulse == &Measurments[DEFAULT_REFERENCE]){
					
					currentPulse->avg = Average(NR_OF_SAMPLES, currentPulse->samples);
					currentPulse->min = Minimum(NR_OF_SAMPLES, currentPulse->samples);
					currentPulse->max = Maximum(NR_OF_SAMPLES, currentPulse->samples);
					
					if(currentPulse->max > maxmax){
						maxmax = currentPulse->max;
					}
					
					if(currentPulse->max < maxmin){
						maxmin = currentPulse->max;
					}
					uart_write((uint8_t) ((maxmax & 0xFF00) >> 8));
					uart_write((uint8_t) (maxmax & 0xFF));
					
// 							uart_write((uint8_t) ((maxmax & 0xFF00) >> 8));
// 					uart_write((uint8_t) ((currentPulse->avg & 0xFF00) >> 8));
// 					uart_write((uint8_t) (currentPulse->avg & 0xFF));
					
					//detectionThreshold = AbsoluteDifference(NR_OF_SAMPLES, currentPulse->samples, Measurments[NOISE_REFERENCE].samples);

					referencePulse = currentPulse;
					currentPulse = &Measurments[NEW_SAMPLE_SET];
					refSample--;
					if(refSample == 0){
						normSample = 1;
						
						uart_write((uint8_t) ((maxmax & 0xFF00) >> 8));
						uart_write((uint8_t) (maxmax & 0xFF));

						uart_write((uint8_t) ((maxmin & 0xFF00) >> 8));
						uart_write((uint8_t) (maxmin & 0xFF));
					}
					
				}
				else{
					currentPulse = &Measurments[DEFAULT_REFERENCE];
				}
			}

			/*transmit data*/			
// 			escapechar = 0;
// 			while(escapechar != 2){
// 				if(uart_writebuffer_ready()){
// 					uart_write(0xff);
// 					escapechar++;
// 				}
// 			}
// 			sendCtr = currentPulse->samples;
// 			while(sendCtr != &(currentPulse->samples[NR_OF_SAMPLES])){
// 				if(uart_writebuffer_ready()){
// 					uart_write(*(uint8_t*) sendCtr);
// 					sendCtr++;
// 				}	
// 			}

			while(UCSR0B & (1 << UDRIE0)); /*wait until  data ready interrupt is turned off (aka, we are done sending data)*/
					
			switch(ADC_select){
				case 1: adc = 1; break;
				case 2: adc = 2; break;
				case 3: adc = 3; break;
				case 4: adc = 4; break;
				default: adc = 0; break;
			}			
//			_delay_ms(1000); /*extra time for syncing, solved a werid bug*/
			_delay_ms(50);
			while((PINC & 0x04) == 0x04); /*Wait until sync pin is low*/
			while((PINC & 0x04) == 0x00); /*Wait until sync pin is High*/
			i = 0;
			if(darkSample){
				while((PINC & 0x04) == 0x04); /*Wait until sync pin is low (light is turned off)*/
				_delay_ms(1); // Just to be sure the light is actually off
			}
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