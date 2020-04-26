/*
 * keypad.h
 *
 * Created: 20-Apr-20 8:24:46 AM
 * Author: Ahmed Gamal
 */ 
#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"

/* 4*3 keypad used */
#define N_col 3
#define N_row 4
#define KEYPAD_PORT_OUT PORTB
#define KEYPAD_PORT_IN  PINB
#define KEYPAD_PORT_DIR DDRB 

/* Function prototype */
uint8 KeyPad_getPressedKey(void);

#endif /* KEYPAD_H_ */
