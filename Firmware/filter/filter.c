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

/*filter coefficients determined by MATLAB (1st ORDER IRR @250Khz with -3DB @10Khz)*/
float a0 = 1; /*normalized and trivial*/
float a1 = -0.7757;
float b0 = 0.1122;
float b1 = 0.1122;

float W0 = 512.5; /*default W0 value for the dark room*/

#define MAX_FP_VAL 2047
#define numofsamples 50
void filter100_IRR(uint16_t * X){
	uint16_t * Y = X;
	float W[numofsamples] = {0};
	//preset first value of W, this should improve settling time.
	W[0] = W0;	
	float Xn;
	uint16_t Yn;
	/*first order IRR filter. Coefficients are global*/
	for(uint16_t n = 1; n < numofsamples; n++){
		Xn = (float) X[n];
		W[n] = (Xn - a1 * W[n-1]);
		Yn = (uint16_t) (b0 * (W[n] + W[n-1])); //as b0 == b1, this stage was simplefied
		Y[n-1] = Yn;
		/*note, this overwrites X[n-1], and can no longer be used*/
	}

	/* Stubbing final Y with previous value to keep an array of 100 values.*/
	Y[numofsamples-1] = Y[numofsamples-2];
}

void filter_CalculateStartupValues(uint16_t * X){
	//create copy, so the original wont be modified
	uint16_t Copy[40];
	memcpy(Copy,X,40);
	filter100_IRR(Copy);
	uint16_t avg = Average(20,&Copy[19]);
	
	W0 = avg;
}