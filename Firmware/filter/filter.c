/*
 * filter.c
 *
 * Created: 26-1-2017 14:16:10
 *  Author: Hajo
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <string.h>
#include "../features/Features.h"
#include "../uart/uart.h"

/*filter coefficients determined by MATLAB (1st ORDER IRR @250Khz with -3DB @10Khz)*/
float a0 = 1; /*normalized and trivial*/
float a1 = -0.7757;
float b0 = 0.1122;
float b1 = 0.1122;

float W0 = 512.5;

uint8_t first = 0;

#define MAX_FP_VAL 2047

void filter100_IRR(uint16_t * X){
	uint16_t * Y = X;
	float W[40] = {0};
	//preset first value of W, this should improve settling time.
	W[0] = W0;	
	float Xn;
	uint16_t Yn;
	/*first order IRR filter. Coefficients are global*/
	for(uint16_t n = 1; n < 40; n++){
		Xn = (float) X[n];
		W[n] = (Xn - a1 * W[n-1]);
		Yn = (uint16_t) (b0 * (W[n] + W[n-1])); //as b1 == b2, this stage was simplefied
// 		uart_write(Yn & 0x00FF);
// 		uart_write((Yn & 0xFF00) >> 8);
		Y[n-1] = Yn;
//		while(UCSR0B & (1 << UDRIE0)); /*wait until  data ready interrupt is turned off (aka, we are done sending data)*/
		/*note, this overwrites X[n-1], and can no longer be used*/
	}
	

	
	/* Stubbing final Y with previous value to keep an array of 100 values.*/
	Y[39] = Y[38];
}

void filter_CalculateStartupValues(uint16_t * X){
	//create copy, so the original wont be modified
	uint16_t Copy[40];
	memcpy(Copy,X,40);
	filter100_IRR(Copy);
	uint16_t avg = Average(20,&Copy[19]);
	
	uart_write((uint8_t) ((avg & 0xFF00) >> 8));
	uart_write((uint8_t) (avg & 0xFF));
	
// 	if(!first){
// 		uart_write((uint8_t) ((Copy[50] & 0xFF00) >> 8));
// 		uart_write((uint8_t) (Copy[50] & 0xFF));
// 		
// 		uart_write((uint8_t) ((Copy[51] & 0xFF00) >> 8));
// 		uart_write((uint8_t) (Copy[51] & 0xFF));
// 		uart_write((uint8_t) ((Copy[52] & 0xFF00) >> 8));
// 		uart_write((uint8_t) (Copy[52] & 0xFF));
// 		uart_write((uint8_t) ((Copy[53] & 0xFF00) >> 8));
// 		uart_write((uint8_t) (Copy[53] & 0xFF));
// 		uart_write((uint8_t) ((Copy[54] & 0xFF00) >> 8));
// 		uart_write((uint8_t) (Copy[54] & 0xFF));
// 		uart_write((uint8_t) ((Copy[55] & 0xFF00) >> 8));
// 		uart_write((uint8_t) (Copy[55] & 0xFF));
// 		uart_write((uint8_t) ((Copy[56] & 0xFF00) >> 8));
// 		uart_write((uint8_t) (Copy[56] & 0xFF));
// 		uart_write((uint8_t) ((Copy[57] & 0xFF00) >> 8));
// 		uart_write((uint8_t) (Copy[57] & 0xFF));
// 		uart_write((uint8_t) ((Copy[58] & 0xFF00) >> 8));
// 		uart_write((uint8_t) (Copy[58] & 0xFF));
// 		uart_write((uint8_t) ((Copy[59] & 0xFF00) >> 8));
// 		uart_write((uint8_t) (Copy[59] & 0xFF));
// 		uart_write((uint8_t) ((Copy[60] & 0xFF00) >> 8));
// 		uart_write((uint8_t) (Copy[60] & 0xFF));
// 		uart_write((uint8_t) ((Copy[61] & 0xFF00) >> 8));
// 		uart_write((uint8_t) (Copy[61] & 0xFF));
// 		uart_write((uint8_t) ((Copy[62] & 0xFF00) >> 8));
// 		uart_write((uint8_t) (Copy[62] & 0xFF));
// 		uart_write((uint8_t) ((Copy[63] & 0xFF00) >> 8));
// 		uart_write((uint8_t) (Copy[63] & 0xFF));
// 		uart_write((uint8_t) ((Copy[64] & 0xFF00) >> 8));
// 		uart_write((uint8_t) (Copy[64] & 0xFF));
// 		
// 		
// 		first ++;
// 	}
	
	W0 = avg;
}