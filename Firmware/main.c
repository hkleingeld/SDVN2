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
#include <stdbool.h>
#include "features/Features.h"
#include "filter/filter.h"
#include "features/StandardDeviation.h"
#include <math.h>
#include <limits.h>
#include <string.h>

/************************************************************************/
/* DEFINITIONS                                                          */
/************************************************************************/
#define NR_OF_SAMPLES 50
#define SIZE_INPUTBUFFER 100
#define SIGMA 4
uint8_t AVG_SAMPLES = 1;


typedef enum state {RESET, LIGHT_INIT, NORMAL_OPERATION} State;
State currentState = RESET;
State nextState = RESET;
/************************************************************************/
/* VARIABLES                                                            */
/************************************************************************/

uint16_t samples[NR_OF_SAMPLES];
uint16_t inputBuffer[SIZE_INPUTBUFFER] = {0};
uint8_t indexinputBuffer = 0;

uint16_t addToBuffer(uint16_t new){
	uint16_t retval = inputBuffer[indexinputBuffer];
	inputBuffer[indexinputBuffer] = new;
	indexinputBuffer++;
	if(indexinputBuffer == SIZE_INPUTBUFFER){
		indexinputBuffer = 0;
	}
	return retval;
}

float calcAvg(void){
	uint16_t tempsum = 0;
	if(indexinputBuffer > AVG_SAMPLES){
		for(int i = 1; i < AVG_SAMPLES+1; i++){
			tempsum += inputBuffer[indexinputBuffer-i];
		}
		return (float) (tempsum/AVG_SAMPLES);
	}
	
	for(int i = 1; i < indexinputBuffer+1; i++){
		tempsum += inputBuffer[indexinputBuffer-i];
	}
	
	for(int i = 0; i < AVG_SAMPLES - indexinputBuffer; i++){
		tempsum += inputBuffer[SIZE_INPUTBUFFER-i-1];
	}
	return (float) (tempsum/AVG_SAMPLES);
}

char itoabuffer[20] = {0};
volatile uint8_t kick = 0;

extern volatile uint8_t ADC_select;

void LightOnSync(void){
	while((PINC & 0x10) == 0x10); /*Wait until sync pin is low*/
	while((PINC & 0x10) == 0x00); /*Wait until sync pin is High - The light will turn very soon :)*/
}

void LightOffSync(void){
	while((PINC & 0x10) == 0x00); /*Wait until sync pin is High*/
	while((PINC & 0x10) == 0x10); /*Wait until sync pin is low - The light has now turned off, and will be off until next cycle*/
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
	uint32_t miscCounter = 0;

	bool objDetLight = false;

	bool darkSample = false;
	uint8_t SamplesTaken = 0;
	
	stdDev * lightMaxStdDev;

	uint16_t lastDeviation = 1000; /*std of 100, is way too much of course :)*/
	/************************************************************************/
	//darkMaxStdDev = StdDev_Init();
	lightMaxStdDev = StdDev_Init();
	
	if(lightMaxStdDev == NULL){
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
// 		uart_write_string(itoa(max,itoabuffer,10));
// 		uart_write(',');
// 		uart_write_string(itoa(samples[NR_OF_SAMPLES-1],itoabuffer,10));
// 		uart_write('\n');

		uint16_t newSample = addToBuffer(max);
	
		switch(currentState){
		case LIGHT_INIT:
			darkSample = false;
	
			if(newSample != 0){
				if(StdDev_Update(lightMaxStdDev,newSample,FORCE_UPDATE) != USHRT_MAX){
					nextState = NORMAL_OPERATION;
				}
			}
			break;
			
		case NORMAL_OPERATION: ; //this semicollomn needs to be here, as you cant define a var directly after a jump...
			//add latest sample to SDT

			uint16_t deviation = StdDev_Update(lightMaxStdDev,newSample,FORCE_UPDATE);
			
			AVG_SAMPLES = pow((deviation * (SIGMA * 10) / StdDev_GetMean(lightMaxStdDev)),2) + 1;
					
			if(AVG_SAMPLES > 99){
				AVG_SAMPLES = 99;
			}
			
			//Average last 3 samples
			float Avg = calcAvg();
			
			if(!objDetLight){
				//if avg > mean+2*std -> detection!
// 				if((Avg > StdDev_GetMean(lightMaxStdDev) + SIGMA* deviation/10) ||
// 				   (Avg < StdDev_GetMean(lightMaxStdDev) - SIGMA* deviation/10))
				if((Avg > StdDev_GetMean(lightMaxStdDev) + 0.01 * StdDev_GetMean(lightMaxStdDev)) ||
				   (Avg < StdDev_GetMean(lightMaxStdDev) - 0.01 * StdDev_GetMean(lightMaxStdDev))
				)
				{
					lastDeviation = deviation;
					objDetLight = true;
					uart_write_string("Object detected!\n");
				}
				uart_write_string(itoa(Avg,itoabuffer,10));
				uart_write_string(" < ");
				uart_write_string(itoa(StdDev_GetMean(lightMaxStdDev),itoabuffer,10));
				uart_write_string(" +- ");
				uart_write_string(itoa(0.01 * StdDev_GetMean(lightMaxStdDev),itoabuffer,10));
				uart_write_string("  noise = ");
				uart_write_string(itoa(deviation,itoabuffer,10));
				uart_write_string(" avg = ");
				uart_write_string(itoa(AVG_SAMPLES,itoabuffer,10));
				uart_write('\n');
			}
			else{
				//optional loop delay (total of stored samples)
				miscCounter++;
				if(miscCounter < SIZE_INPUTBUFFER + AVG_SAMPLES + 10) break; /*ensure the event is in the STD before trying to resolve it the + 10 is weird but it works now..*/
				
				if(miscCounter % 200 == 0) lastDeviation++; /*why 200? no clue*/
				//if current std value < last std value -> no detection!
				if(deviation < lastDeviation){
					objDetLight = false;
					uart_write_string("Looking for objects...\n");
					miscCounter = 0;
				}
				else{
					uart_write_string(itoa(deviation,itoabuffer,10));
					uart_write_string(" > ");
					uart_write_string(itoa(lastDeviation,itoabuffer,10));
					uart_write('\n');
				}
			}
			
			//uint16_t SNR1000 = StdDev_GetMean(lightMaxStdDev) / (3 * deviation); //snr 1000 = 1000*snr , SNR = Mean * 0.01 / (3 * deviation)
			//				1		10			1		1
			
			break;
		case RESET:
			nextState = LIGHT_INIT;
			StdDev_Reset(lightMaxStdDev);
			uart_write_string("Starting light init\n");
			break;
		default:
			nextState = RESET;
			break;
		}

		//check for instructions
		if(currentState != nextState){
			SamplesTaken = 0;
			currentState = nextState;
		}
		
		/*turn light on or off based on objDetLight*/
		if(objDetLight){
			//turn lights on
		}
		else{
			//turn lights off
		}

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