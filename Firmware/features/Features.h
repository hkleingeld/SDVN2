/*
 * Features.h
 *
 * Created: 9-12-2016 12:08:11
 *  Author: Hajo
 *
 * Contains several functions to get certain features from uint16 arrays
 */
#ifndef _FEATURES_H_GUARD_
#define _FEATURES_H_GUARD_

#include <stdint.h>

uint16_t Minimum(uint16_t size, uint16_t * array);
uint16_t Maximum(uint16_t size, uint16_t * array);
uint16_t Average(uint16_t size, uint16_t * array);

uint16_t AbsoluteDifference(uint16_t size, uint16_t * array1, uint16_t * array2);

uint16_t PulseLenght(uint16_t size, uint16_t threshold, uint16_t * array);
uint16_t PulseStart(uint16_t size, uint16_t threshold, uint16_t * array);
uint16_t PulseEnd(uint16_t size, uint16_t threshold, uint16_t * array);

#endif