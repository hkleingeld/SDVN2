/*
 * Features.c
 *
 * Created: 9-12-2016 12:07:56
 *  Author: Hajo
 */
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>


uint16_t Minimum(uint16_t size, uint16_t * array){
	uint16_t min = USHRT_MAX;
	
	for(int i = 0; i < size ;i++){
		if(array[i] < min){
			min = array[i];
		}
	}
		
	return(min);
}

uint16_t Maximum(uint16_t size, uint16_t * array){
	uint16_t max = 0;
	
	for(int i = 0; i < size ; i++){
		if(array[i] > max){
			max = array[i];
		}
	}
	
	return(max);
}

uint16_t Average(uint16_t size, uint16_t * array){
	uint32_t sum = 0;
	
	for(int i = 0; i < size ; i++){
		sum += array[i];
	}
	
	return(sum/size);
}

uint16_t AbsoluteDifference(uint16_t size, uint16_t * array1, uint16_t * array2){
	uint32_t AbsDiff = 0;
	
	for(int i = 0; i < size ; i++){
		//I'm allowed to cast the uint16 numbers to int as their maximum value is 3500
		AbsDiff += abs((int32_t) array1[i] - (int32_t) array2[i]);
	}
	
	return(AbsDiff);
}

uint16_t PulseLenght(uint16_t size, uint16_t threshold, uint16_t * array){
	uint16_t BaseSample = array[0];
	uint16_t StartPulse = 0;
	
	for(int i = 1; i < size; i++){
		//I'm allowed to cast the uint16 numbers to int as their maximum value is 3500
		uint16_t diff = abs((int16_t) BaseSample - (int16_t) array[i]);
		if(diff > threshold){
			if(StartPulse == 0){
				StartPulse = i;
			}
			continue;
		}
		
		if(StartPulse != 0){
			return(i - StartPulse);
		}
	}
	return 0;
}

uint16_t PulseStart(uint16_t size, uint16_t threshold, uint16_t * array){
	uint16_t BaseSample = array[0];
	
	for(int i = 1; i < size; i++){
		//I'm allowed to cast the uint16 numbers to int as their maximum value is 3500
		uint16_t diff = abs((int16_t) BaseSample - (int16_t) array[i]);
		if(diff > threshold){
			return i;
		}
	}
	return 0;
}

uint16_t PulseEnd(uint16_t size, uint16_t threshold, uint16_t * array){
	uint16_t BaseSample = array[0];
	uint16_t StartPulse = 0;
	
	for(int i = 1; i < size; i++){
		//I'm allowed to cast the uint16 numbers to int as their maximum value is 3500
		uint16_t diff = abs((int16_t) BaseSample - (int16_t) array[i]);
		if(diff > threshold){
			if(StartPulse == 0){
				StartPulse = i;
			}
			continue;
		}
		
		if(StartPulse != 0){
			return(i);
		}
	}
	return 0;
}