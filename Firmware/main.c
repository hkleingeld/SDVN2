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
#include "features/StandardDeviation.h"
#include <math.h>
#include <limits.h>
#include <string.h>

/************************************************************************/
/* DEFINITIONS                                                          */
/************************************************************************/
#define NR_OF_SAMPLES 40

typedef enum state {RESET, DARK_INIT, LIGHT_INIT, NORMAL_OPERATION} State;
State currentState = RESET;
State nextState = RESET;
/************************************************************************/
/* VARIABLES                                                            */
/************************************************************************/

uint16_t samples[NR_OF_SAMPLES];

char itoabuffer[20] = {0};
volatile uint8_t kick = 0;

extern volatile uint8_t ADC_select;

void LightOnSync(void){
	while((PINC & 0x04) == 0x04); /*Wait until sync pin is low*/
	while((PINC & 0x04) == 0x00); /*Wait until sync pin is High - The light will turn very soon :)*/
}

void LightOffSync(void){
	while((PINC & 0x04) == 0x00); /*Wait until sync pin is High*/
	while((PINC & 0x04) == 0x04); /*Wait until sync pin is low - The light has now turned off, and will be off until next cycle*/
}

void Error(const char * error_msg){
	uart_write_string("\nCritical Error: ");
	uart_write_string(error_msg);

	while(1); //something went horribly wrong. Perma wait to avoid any damage :O!
}

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
int main(void)
{
	uint8_t adc = ADC_select;
	int i = 0;
	
	uint8_t n = 0;
	uint8_t objDetLight = 0;
	
	uint8_t j = 0;
	uint8_t objDetDark = 0;

	uint8_t darkSample = 1;
	uint8_t SamplesTaken = 0;
	
	stdDev * darkMaxStdDev;
	stdDev * lightMaxStdDev;

	/************************************************************************/
	//darkMaxStdDev = StdDev_Init();
	lightMaxStdDev = StdDev_Init();
	
	if(/*(darkMaxStdDev == NULL) ||*/ (lightMaxStdDev == NULL)){
		Error("malloc of stddev failed"); /*malloc of stddev did somehow fail :'( */
	}
	
	//timer1_init();	// Set up Timer 1.
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
	uart_write('\n');
	/************************************************************************/
	
	receiver_measure();		// Do a medium measurement.
	receiver_reset();		// Reset the receiver.
	/************************************************************************/
	

	while(1){
		switch(ADC_select){ /*PD select, This ensures the selection cant change during sampling*/
			case 1: adc = 1; break;
			case 2: adc = 2; break;
			case 3: adc = 3; break;
			case 4: adc = 4; break;
			default: adc = 0; break;
		}
		
		LightOnSync(); /*sync*/
		i = 0;
		if(darkSample){
			LightOffSync(); /*Wait until sync pin is low (light is turned off)*/
			_delay_ms(1); // Just to be sure the light is actually off
		}
		
		do{ /*sample*/
			switch(adc){
				case 1: samples[i] = spi_adc_read(ADC_CHANNEL1); break;
				case 2: samples[i] = spi_adc_read(ADC_CHANNEL2); break;
				case 3: samples[i] = spi_adc_read(ADC_CHANNEL3); break;
				case 4: samples[i] = spi_adc_read(ADC_CHANNEL4); break;
				default: break;
			}
			i++;
		} while(i < NR_OF_SAMPLES);

		SamplesTaken++;
		//Filter
// 		for(int j=0; j<NR_OF_SAMPLES ; j++){
// 			uart_write(samples[j] & 0xFF);
// 			uart_write(samples[j]>>8 & 0xFF);
// 		}
		filter100_IRR(&samples[0]);

		//Extract features
		uint16_t max = Maximum(NR_OF_SAMPLES, &samples[0]);
//  		uart_write_string(itoa(max,itoabuffer,10));
// 		uart_write(' ');
	
		switch(currentState){
// 		case DARK_INIT:
// 			if(StdDev_Update(darkMaxStdDev,max,FORCE_UPDATE) != USHRT_MAX){
// 				if(StdDev_GetStdDev(darkMaxStdDev) < 15){ //stddev is too high, keep going.
// 					nextState = LIGHT_INIT;
// 					darkSample = 0;
// 					uart_write_string("Starting Light Init\n");
// 				}
// 								
// 			}	
// 			break;
		case LIGHT_INIT:
			darkSample = 0;
			if(StdDev_Update(lightMaxStdDev,max,FORCE_UPDATE) != USHRT_MAX){
				if(StdDev_GetStdDev(lightMaxStdDev) < 15){ //stddev is too high, keep going.
					nextState = NORMAL_OPERATION;
					uart_write_string("Starting Normal opperation\n");
				}
			}
			break;
		case NORMAL_OPERATION:
			switch(darkSample){
				case 0: //light sample
					//darkSample = 1;
					
					if(objDetLight){
						if(StdDev_Update(lightMaxStdDev,max,FORCE_UPDATE) != USHRT_MAX){
							if(StdDev_GetStdDev(lightMaxStdDev) < 15){
								objDetLight = 0;
								uart_write_string("\n\n\nOK\n\n\n");
							}
						}
					}
					else{
						if(StdDev_Update(lightMaxStdDev,max,0) < 31){ //if newly added sample is whithin 3 deviations
							n = 0;
						}
						else{
							if(n++ == 1){
								objDetLight = 1;
								StdDev_Reset(lightMaxStdDev);
								n = 0;
							}
						}
					}
					uart_write_string(itoa(n,itoabuffer,10));

				break;
				
				case 1: //dark sample
					darkSample = 0;
					if(objDetDark){
						if(StdDev_Update(darkMaxStdDev,max,FORCE_UPDATE) != SHRT_MAX){
							if(StdDev_GetStdDev(darkMaxStdDev) < 15){
								objDetDark = 0;
							}
						}
					}
					else{
						if(StdDev_Update(darkMaxStdDev,max,0) < 31){ //if newly added sample is whithin 3 deviations
							j = 0;
						}
						else{
							if(j++ == 1){
								objDetDark = 1;
								StdDev_Reset(darkMaxStdDev);
								j = 0;
							}
						}
					}
				break;
				}	
			break;
		case RESET:
			nextState = LIGHT_INIT;
			//StdDev_Reset(darkMaxStdDev);
			StdDev_Reset(lightMaxStdDev);
			uart_write_string("Starting dark init\n");
			break;
		default:
			nextState = RESET;
			break;
		}

		if(objDetDark == 1){
			uart_write('D');
		}
		
		if(objDetLight == 1){
			uart_write('L');
			uart_write('\n');
		}
		
		//check for instructions
		if(currentState != nextState){
			SamplesTaken = 0;
			currentState = nextState;
		}
		/*turn light on or off based on objDetLight and objDetDark*/
		
		_delay_ms(50);
		while(UCSR0B & (1 << UDRIE0)); /*wait until  data ready interrupt is turned off (aka, we are done sending data)*/
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