/*
 * uart.h
 *
 * Created: 22-6-2017 14:45:04
 *  Author: Hajo
 */
#ifndef _UART_H_GUARD_
#define _UART_H_GUARD_

#include <interrupt.h>

uint8_t uart_send_char(char new_char);
uint8_t uart_send_string(const char * string);
void uart_setup(void);

#endif