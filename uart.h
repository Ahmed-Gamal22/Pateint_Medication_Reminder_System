/*
 * uart.h
 *
 * Created: 18-Apr-20 2:27:27 PM
 * Author: Ahmed Gamal
 */
#ifndef UART_H_
#define UART_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void UART_init(void);
void UART_sendByte(const uint8 data);
uint8 UART_recieveByte(void);
void UART_sendString(const uint8 *Str);
void UART_receiveString(uint8 *Str); // Receive until #

#endif 