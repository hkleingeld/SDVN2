
/*
 * StandardDeviation.c
 *
 * Created: 21-2-2017 16:31:05
 * Author: Hajo
 * All calculations based upon: https://www.mathsisfun.com/data/standard-normal-distribution.html
 */ 

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>


#define NR_OF_STD_SAMPLES 128

#include "StandardDeviation.h"
#include "../uart/uart.h"
#include <avr/io.h>

static float Variance(stdDev * this){
	float mean = this->sum/NR_OF_STD_SAMPLES;
	
	float variance = 0;
	for(int i = 0; i < NR_OF_STD_SAMPLES; i++){
		variance += pow((float) this->StdSampleList[i] - mean,2);
	}
	
	return(variance);
}

static void updateSum(stdDev * this, uint16_t newSample){
	this->sum -= this->StdSampleList[this->listIndex];
	this->sum += newSample;
	this->StdSampleList[this->listIndex] = newSample;
	
	this->listIndex++;
	if(this->listIndex == NR_OF_STD_SAMPLES){
		this->listIndex = 0;
	}
}

static uint32_t Sum(stdDev * this){
	uint32_t sum = 0;
	for(int i = 0; i < NR_OF_STD_SAMPLES ; i++){
		sum += this->StdSampleList[i];
	}

	return(sum);
}

void StdDev_Reset(stdDev * this){
	this->StdDev = 0;
	this->sum = 0;
	this->variance = 0;
	this->active = 0;
	this->listIndex = 0;
	memset(this->StdSampleList,0,sizeof(uint16_t) * NR_OF_STD_SAMPLES);
}

stdDev * StdDev_Init(void){
	stdDev * this = malloc(sizeof(stdDev));
	if(this == NULL){
		return(NULL);
	}
	
	this->StdSampleList = malloc(sizeof(uint16_t) * NR_OF_STD_SAMPLES);
	if(this->StdSampleList == NULL){
		return(NULL);
	}

	StdDev_Reset(this);
	return(this);
}

void StdDev_Delete(stdDev * this){
	
	StdDev_Reset(this);
	free(this->StdSampleList);
	free(this);
}

void StdDev_setPop(stdDev * this){
	this->sum = Sum(this);
	this->variance = Variance(this);
	this->StdDev = sqrt(this->variance/NR_OF_STD_SAMPLES);
}

float StdDev_GetStdDev(stdDev * this){
	return(this->StdDev);
}

float StdDev_GetMean(stdDev * this){
	return(this->sum/NR_OF_STD_SAMPLES);
}

uint16_t StdDev_Update(stdDev * this, uint16_t newSample, uint8_t settings){
	if(this->active){
		float mean = this->sum / NR_OF_STD_SAMPLES;
		float sampleDeviation = newSample - mean / this->StdDev;

		if( (fabsf(sampleDeviation) < (this->StdDev * 3)) || 
		    ((settings & FORCE_UPDATE) == FORCE_UPDATE))
			{ //if sample is in range or if forced, update!
			updateSum(this, newSample);
			this->variance = Variance(this);
			this->StdDev = sqrt(this->variance/NR_OF_STD_SAMPLES);
		}
		else if((settings & FORCE_UPDATE) == FORCE_UPDATE){
			
		}
		return (sampleDeviation*10);
	}
	else{
		if(this->listIndex == NR_OF_STD_SAMPLES){
			this->active = 1;
		}
		updateSum(this, newSample);
		return(USHRT_MAX);
	}
}