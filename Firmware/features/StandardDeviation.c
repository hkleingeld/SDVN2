
/*
 * StandardDeviation.c
 *
 * Created: 21-2-2017 16:31:05
 * Author: Hajo
 * All calculations based upon: https://www.mathsisfun.com/data/standard-normal-distribution.html
 */ 

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>

#include "StandardDeviation.h"

static float Variance(stdDev * this, uint16_t size, uint16_t * array){
	float variance = 0;
	for(int i = 0; i < size; i++){
		variance = pow(this->StdDevMean - (float) array[i],2);
	}
	
	return(variance);
}

static float Avg(stdDev * this, uint16_t size, uint16_t * array){
	uint32_t sum = 0;
	float sum_f = 0;
	for(int i = 0; i < size ; i++){
		sum += array[i];
	}
	sum_f = (float) sum;
	return(sum_f/size);
}

void StdDev_Reset(stdDev * this){
	this->StdDev = 0;
	this->StdDevMean = 0;
	this->StdDevVariance = 0;
	this->StdDevSamplesInPop = 0;
}

stdDev * StdDev_Init(void){
	stdDev * this = malloc(sizeof(stdDev));
	if(this == NULL){
		return(NULL);
	}
	StdDev_Reset(this);
	return(this);
}

void StdDev_Delete(stdDev * this){
	StdDev_Reset(this);
	free(this);
}

void StdDev_setPop(stdDev * this, uint16_t size, uint16_t * array){
	this->StdDevMean = Avg(this, size, array);
	this->StdDevVariance = Variance(this, size, array);
	this->StdDev = sqrt(this->StdDevVariance/size);
	this->StdDevSamplesInPop = size;
}

void StdDev_AddSample(stdDev * this, uint16_t newSample){
	this->StdDevSamplesInPop++;
	this->StdDevMean += ((float)newSample) / this->StdDevSamplesInPop;
	this->StdDevVariance += pow(this->StdDevMean - newSample,2);
	this->StdDev = sqrt(this->StdDevVariance/this->StdDevSamplesInPop);
}

int8_t StdDev_GetDeviation(stdDev * this, uint16_t newSample){
	float sample = (float) newSample;
	return (int8_t) (((sample - this->StdDevMean)/this->StdDev)*10);
}

float StdDev_GetStdDev(stdDev * this){
	return(this->StdDev);
}