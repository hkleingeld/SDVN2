/*
 * fifo.h
 *
 * Created: 2-5-2017 17:38:42
 *  Author: Hajo
 */ 

#ifndef _FIFO_H_GUARD_
#define _FIFO_H_GUARD_

#define FIFO_UNDERFLOW 2
#define FIFO_OVERFLOW 1
#define FIFO_OK 0

#include <stdint.h>

typedef struct FIFO{
	uint16_t size;
	int16_t WriteIndex;
	int16_t ReadIndex;
	uint8_t * Array;
} FIFO;


FIFO * FIFOInit(uint16_t size);
void FIFODelete(FIFO * This);

uint8_t FIFOPush(FIFO * This, uint8_t NewVal);
uint8_t FIFOPull(FIFO * This, uint8_t * NewVal);

#endif
