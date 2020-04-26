/*
 * keypad.c
 *
 * Created: 20-Apr-20 8:24:46 AM
 * Author: Ahmed Gamal
 */ 
#include "keypad.h"

/* static function prototype */
static uint8 KeyPad_4x3_adjustKeyNumber(uint8 button_number);

/********************************************************************************************************
 *[Function Name]: KeyPad_getPressedKey  		     													*
 *[Description]: This function is responsible for get the key presed by the user in 4*3 keypad.			*
 *[In]: void																							*
 *[Out]: (unsigned char) key																			*
 ********************************************************************************************************/
uint8 KeyPad_getPressedKey(void){
	uint8 col,row;
	while(1)
	{
		/* loop for columns */
		for(col=0;col<N_col;col++) 
		{
			KEYPAD_PORT_DIR = (0b00010000<<col); 
			KEYPAD_PORT_OUT = (~(0b00010000<<col)); 
			/* loop for rows */
			for(row=0;row<N_row;row++) 
			{
				/* if the switch is press in this row */ 
				if(BIT_IS_CLEAR(KEYPAD_PORT_IN,row)) 
				{	
					return KeyPad_4x3_adjustKeyNumber((row*N_col)+col+1); 				
				}
			}
		}
	}	
}

/**********************************************************************************************************
 *[Function Name]: KeyPad_4x3_adjustKeyNumber  		     												  *
 *[Description]: This function is responsible for get the key pressed by the user in 4*3 keypad and return*
 *				 the special ASCII representative the hardware keypad layout.							  *
 *[In]: (unsigned char) button_number																	  *
 *[Out]: (unsigned char)																				  *
 **********************************************************************************************************/
static uint8 KeyPad_4x3_adjustKeyNumber(uint8 button_number)
{
	switch(button_number)
	{
		case 10: return '*'; 
				 break;
		case 11: return 0;
				 break;		
		case 12: return '#'; 
				 break;
		default: return button_number;								 						
	}
} 

