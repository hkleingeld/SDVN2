/*
 * transmitter.c
 *
 * Created: 25-11-2014 9:45:09
 *  Author: Lennart
 */ 
/************************************************************************/
/* LIBRARIES                                                            */
/************************************************************************/
#include "transmitter.h"

/************************************************************************/
/* DEFINITIONS                                                          */
/************************************************************************/
typedef enum {TXSTATE_DISABLED, TXSTATE_SIGNALRISE, TXSTATE_SIGNALFALL, TXSTATE_SIGNALLOW} TXSTATE;
	
/************************************************************************/
/* VARIABLES                                                            */
/************************************************************************/
RingBuff_t ring_buffer_data;

// Current selected transmission direction.
uint16_t LedTargetTime = 0; /*time = Total ticks on*/
uint8_t LapsToGo = 0;
extern volatile uint8_t receiverenabled;

uint8_t txLed8_1	= 0b01110000;
uint8_t txLed16_9	= 0x00;
uint8_t txLed20_17	= 0x00;

TXSTATE TX1state = TXSTATE_DISABLED;



/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/

/**
 * Process the transmitter state machine at a predefined interval.
 */
void transmitter(void) {
	static uint8_t PwmCoef = 0;
	static uint8_t PwmCtr = 0;
	
	if(RingBuffer_IsEmpty(&ring_buffer_data)) {		// Check for new PWM coefficient
													// No? Don't change anything
	}
	else {
		PwmCoef = RingBuffer_Remove(&ring_buffer_data);	// Yes, load new PWM coefficient
		PwmCtr = 0;
		uart_write(PwmCoef);
	}
	
	if(PwmCtr == 0){ // start with turning lights on 
		if(PwmCoef != 0) // make sure light does not turn on if light is meant to be off
		{
			spi_gpio_write(GPIO_BANK_LED8_1,	txLed8_1);	// Set the selected leds on.
			spi_gpio_write(GPIO_BANK_LED16_9,	txLed16_9);	// Set the selected leds on.
			spi_gpio_write(GPIO_BANK_LED20_17,	txLed20_17);// Set the selected leds on.
		}
	}

	if(PwmCtr == PwmCoef){
		spi_gpio_write(GPIO_BANK_LED8_1,	0);	// Set the leds off.
		spi_gpio_write(GPIO_BANK_LED16_9,	0);	// Set the leds off.
		spi_gpio_write(GPIO_BANK_LED20_17,	0);	// Set the leds off.
	}
	
	PwmCtr++;
	if(PwmCtr == 10){ // Makes sure that lights remain on if intensity is set too 10.
		PwmCtr = 0;
	}

}

/**
 * Initialize the transmitter.
 */
void transmitter_init(void) {

	RingBuffer_InitBuffer(&ring_buffer_data); // initialize the buffer.
}

/* Functions regarding the data buffer. */

/**
 * Add a byte to the transmitter queue.
 */
void transmitter_add(uint8_t data) {
	
	if(!RingBuffer_IsFull(&ring_buffer_data)) {
		RingBuffer_Insert(&ring_buffer_data, data);	// Put data in the buffer.
	} else {
		//TODO: handle a full buffer. Currently we just discard the data.
	}
}

/**
 * Set the direction to transmit to.
 */
void transmitter_setdirection(uint8_t led8_1, uint8_t led16_9, uint8_t led20_17) {
	txLed8_1	= led8_1;
	txLed16_9	= led16_9;
	txLed20_17	= led20_17;
}

void set_pulse_time(uint8_t targetTime){
	LedTargetTime = targetTime * 2 + 5;
	LapsToGo = 0;
}
