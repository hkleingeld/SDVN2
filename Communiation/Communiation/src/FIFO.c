/*
 * CFile1.c
 *
 * Created: 2-5-2017 17:36:13
 *  Author: Hajo
 */ 

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "FIFO.h"

uint16_t FIFIDataInBuffer(FIFO * This){
	
}

static uint16_t IncrementPointer(FIFO * This, uint16_t val){
	val++;
	if(val == This->size){
		val = 0;
	}
	return val;
}

static uint16_t DecrementPointer(FIFO * This, uint16_t val){
	if(val == 0){
		val = This->size;
	}
	return val--;
}

uint8_t FIFOPush(FIFO * This, uint8_t NewVal){
	This->WriteIndex = IncrementPointer(This, This->WriteIndex);
	if(This->WriteIndex == This->ReadIndex){
		/*overflow, Don't do it and decrease write pointer by 1*/
		This->WriteIndex = DecrementPointer(This, This->WriteIndex);
		return FIFO_OVERFLOW;
	}
	
	This->Array[This->WriteIndex] = NewVal;
	
	return FIFO_OK;
}

uint8_t FIFOPull(FIFO * This, uint8_t * NewVal){
	if(This->ReadIndex == This->WriteIndex){
		return FIFO_UNDERFLOW;
	}
	This->ReadIndex = IncrementPointer(This, This->ReadIndex);
	*NewVal = This->Array[This->ReadIndex];
	
	return FIFO_OK;
}

FIFO * FIFOInit(uint16_t size){
	FIFO * F = (FIFO *) malloc(sizeof(FIFO));
	if(F == NULL) return NULL;

	F-> Array = (uint8_t *) malloc(sizeof(uint8_t) * size);
	if(F == NULL) return NULL;

	memset(F->Array, 0xFF, sizeof(uint8_t) * (size));

	F->size = size;
	F->WriteIndex = 0;
	F->ReadIndex = 0;

	return(F);
}

void FIFODelete(FIFO * This){
	free(This->Array);
	free(This);
}

