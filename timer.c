/*
 * timer.c
 *
 * Created: 13-Apr-20 9:43:39 AM
 * Author: Ahmed Gamal
 */ 
#include "timer.h"

/********************************************************************************************************
 *[Function Name]: timer_Init  		     																*
 *[Description]: This function is responsible for initialization timer module in compare match mode.	*
 *[In]: void																							*
 *[Out]: void																							*
 ********************************************************************************************************/
void timer_Init(void)
{
	/* CTC mode */
	TCCR1B|=(1<<WGM12);
	OCR1A = 65500; 
	TCNT1 = 0;
}

/********************************************************************************************************
 *[Function Name]: timer_enable  		     															*
 *[Description]: This function is responsible for enable timer module clock to start working.			*
 *[In]: void																							*
 *[Out]: void																							*
 ********************************************************************************************************/
void timer_enable(void)
{
	/* 1024 prescaler */
	TCCR1B|=(1<<CS10)|(1<<CS12);
	/* timer interrupt enable */
	TIMSK|=(1<<OCIE1A);
}