/*
 * SuperBlink.c
 *
 * Created: 22-6-2017 16:32:50
 *  Author: Hajo
 */ 

#include "SuperBlink.h"
#include <asf.h>

uint16_t TonTime;
uint16_t Period;
uint8_t LedNr;

void superblink_set_ton(uint16_t val){
	TonTime = val;
}

void superblink_set_Period(uint16_t val){
	Period = val;
}

void superblink_set_LedNr(uint8_t val){
	LedNr = val;
}