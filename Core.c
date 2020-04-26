/*
 * Core.c
 *
 * Created: 20-Apr-20 8:25:49 AM
 * Author: Ahmed Gamal
 */
#include "PatientReminderSys.h"
/* global state for FSM design */
volatile state_t currentState = idle;
volatile uint8 counter = 0;
void main(void)
{
	/* initialization required modules: UART, i2c, buzzer, LCD, external interrupt, timer */
	sysInit();
	/* super loop */
	while(1)
	{
		switch(currentState)
		{
			case idle:
			idleMode();
			break;
			case addMedcine:
			addNewMedcine();
			currentState = nearestMed;
			break;
			case nearestMed:
			calcNearestMed();
			currentState = idle;
			break;
			case updateTime:
			if(medTimeCheck())
			{
				currentState = takeAction;
			}
			else
			{
				currentState = idle;
			}
			break;
			case takeAction:
			action_on();
			currentState = doNothing;
			break;
			case doNothing:
			/* Do Nothing */
			break;
			case stopAlert:
			action_off();
			currentState = nearestMed;
			break;
		}
	}
}
/* External INT1 Interrupt Service Routine */
ISR(INT1_vect)
{
	currentState = addMedcine;
}
/* External INT0 Interrupt Service Routine */
ISR(INT0_vect)
{
	currentState = stopAlert;
}
/* Timer1 compare match Service Routine */
ISR(TIMER1_COMPA_vect)
{
	counter++;
	if(counter == 5)
	{
		counter = 0;
		currentState = updateTime;
	}
}