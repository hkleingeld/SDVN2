/*
 * SuperBlink.h
 *
 * Created: 22-6-2017 16:33:01
 *  Author: Hajo
 */ 
#ifndef _SUPERBLINK_H_GUARD_
#define _SUPERBLINK_H_GUARD_
#include <asf.h>

void superblink_set_Period(uint16_t val);
void superblink_set_ton(uint16_t val);
void superblink_set_LedNr(uint8_t val);

#endif