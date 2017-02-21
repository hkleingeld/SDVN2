
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

static float StdDev = 0;
static float StdDevMean = 0;
static float StdDevVariance = 0;
static uint16_t StdDevSamplesInPop = 0;

static float Variance(uint16_t size, uint16_t * array){
	float variance = 0;
	for(int i = 0; i < size; i++){
		variance = pow(StdDevMean - (float) array[i],2);
	}
	
	return(variance);
}

static float Avg(uint16_t size, uint16_t * array){
	uint32_t sum = 0;
	float sum_f = 0;
	for(int i = 0; i < size ; i++){
		sum += array[i];
	}
	sum_f = (float) sum;
	return(sum_f/size);
}

void StdDev_Reset(void){
	StdDev = 0;
	StdDevMean = 0;
	StdDevVariance = 0;
	StdDevSamplesInPop = 0;
}

void StdDev_setPop(uint16_t size, uint16_t * array){
	StdDevMean = Avg(size, array);
	StdDevVariance = Variance(size, array);
	StdDev = sqrt(StdDevVariance/size);
	StdDevSamplesInPop = size;
}

void StdDev_AddSample(uint16_t newSample){
	StdDevSamplesInPop++;
	StdDevMean += ((float)newSample) / StdDevSamplesInPop;
	StdDevVariance += pow(StdDevMean - newSample,2);
	StdDev = sqrt(StdDevVariance/StdDevSamplesInPop);
}

int8_t StdDev_GetDeviation(uint16_t newSample){
	float sample = (float) newSample;
	return (int8_t) (((sample - StdDevMean)/StdDev)*10);
}

float StdDev_GetStdDev(void){
	return(StdDev);
}