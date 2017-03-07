/*
 * StandardDeviation.h
 *
 * Created: 21-2-2017 16:30:56
 *  Author: Hajo
 */ 
#ifndef _STDDEV_H_GUARD_
#define _STDDEV_H_GUARD_

#include <stdint.h>

#define FORCE_UPDATE 0b00000001

typedef struct stdDev{
	float StdDev;
	float sum;
	float variance;
	uint16_t * StdSampleList;
	uint8_t active;
	uint8_t listIndex;
} stdDev;

void StdDev_Reset(stdDev * this);
void StdDev_setPop(stdDev * this);

int8_t StdDev_GetDeviation(stdDev * this, uint16_t newSample);
float StdDev_GetStdDev(stdDev * this);
uint16_t StdDev_Update(stdDev * this, uint16_t newSample, uint8_t settings);

stdDev * StdDev_Init(void);
void StdDev_Reset(stdDev * this);
void StdDev_Delete(stdDev * this);

#endif