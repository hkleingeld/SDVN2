
/*
 * StandardDeviation.h
 *
 * Created: 21-2-2017 16:30:56
 *  Author: Hajo
 */ 
#ifndef _STDDEV_H_GUARD_
#define _STDDEV_H_GUARD_

#include <stdint.h>

void StdDev_Reset(void);
void StdDev_setPop(uint16_t size, uint16_t * array);
void StdDev_AddSample(uint16_t newSample);

int8_t StdDev_GetDeviation(uint16_t newSample);
float StdDev_GetStdDev(void);

#endif