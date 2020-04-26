/*
 * exInterrupt.c
 *
 * Created: 19-Apr-20 1:27:06 PM
 * Author: Ahmed Gamal
 */ 
#include "exInterrupt.h"

/********************************************************************************************************
 *[Function Name]: EXINT_init  		     																*
 *[Description]: This customization function is responsible for initialization External Interrupt 0		*
 *				 and External Interrupt 1.																*
 *[In]: void																							*
 *[Out]: void																							*
 ********************************************************************************************************/
void EXINT_init(void)
{
	/* Configure INT1/PD3,INT0/PD2 as input pins */
	DDRD  &= (~(1<<PD3))&(~(1<<PD2));
	/* Enable the internal pull up resistor at PD3,PD2 pins */  
	PORTD |= (1<<PD3)|(1<<PD2);     
	/* Enable external interrupt pin INT1,INT0 */
	GICR  |= (1<<INT1)|(1<<INT0);    
	/* Trigger INT1,INT0 with the falling edge */
	MCUCR |= (1<<ISC11)|(1<<ISC01);
	MCUCR &= ~(1<<ISC10)&(~(1<<ISC00));
}