
/*
 * StandardDeviation.h
 *
 * Created: 21-2-2017 16:30:56
 *  Author: Hajo
 */ 
#ifndef _STDDEV_H_GUARD_
#define _STDDEV_H_GUARD_

#include <stdint.h>

typedef struct stdDev{
	float StdDev;
	float StdDevMean;
	float StdDevVariance;
	uint16_t StdDevSamplesInPop;
} stdDev;

void StdDev_Reset(stdDev * this);
void StdDev_setPop(stdDev * this, uint16_t size, uint16_t * array);
void StdDev_AddSample(stdDev * this, uint16_t newSample);

int8_t StdDev_GetDeviation(stdDev * this, uint16_t newSample);
float StdDev_GetStdDev(stdDev * this);

void StdDev_Reset(stdDev * this);
stdDev * StdDev_Init(void);
void StdDev_Delete(stdDev * this);

#endif