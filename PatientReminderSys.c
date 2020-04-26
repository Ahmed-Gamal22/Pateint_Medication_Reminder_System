/*
 * PatientReminderSys.c
 *
 * Created: 18-Apr-20 2:27:27 PM
 * Author: Ahmed Gamal
 */ 
#include "PatientReminderSys.h"

/* array of structures defines the structure used for all medicines in the project */
static medecine_t medecineArr[MAX_MEDECINE_NUM]; 
/* nearsestMedIndex_G defines which medicine is nearest int time to be taken */
static uint8 nearsestMedIndex_G = 0;   
/* medCounter defines number of medicine already in the array used in calculation which is the nearest one */
static uint8 medCounter = 0;		   

/********************************************************************************************************
 *[Function Name]: sysInit  		     																*
 *[Description]: This function is responsible for initialization RTC,UART,LCD,external interrupts,timer,*
 *				 Buzzer modules required for main purpose of project.									*
 *[In]: void																							*
 *[Out]: void																							*
 ********************************************************************************************************/
void sysInit(void)
{
	UART_init();
	LCD_init();
	RTC_init();
	sei();
	EXINT_init();
	timer_Init();
	/* Buzzer initialization Off starting */
	DDRD|=(1<<PD7);
	PORTD&=~(1<<PD7);
}

/********************************************************************************************************
 *[Function Name]: action_on  		     																*
 *[Description]: This function is responsible for taking action by making buzzer on and print name of   *
 *				 medicine on LCD.																		*
 *[In]: void																							*
 *[Out]: void																							*
 ********************************************************************************************************/
void action_on(void)
{
	LCD_clearScreen();
	LCD_goToRowColumn(0,0);
	LCD_displayString("Med Time");
	LCD_goToRowColumn(1,0);
	LCD_displayString(medecineArr[nearsestMedIndex_G].medName);
	_delay_ms(1000);
	/* Buzzer ON */
	PORTD|=(1<<PD7); 
}

/********************************************************************************************************
 *[Function Name]: action_off  		     																*
 *[Description]: This function is responsible for taking action by making buzzer off and print ALERT Off*
 *				 on LCD.																				*
 *[In]: void																							*
 *[Out]: void																							*
 ********************************************************************************************************/
void action_off(void)
{
	LCD_clearScreen();
	LCD_goToRowColumn(0,0);
	LCD_displayString("ALERT OFF");
	_delay_ms(1000);
	/* Buzzer Off */
	PORTD&=~(1<<PD7);
	LCD_clearScreen();
}

/********************************************************************************************************
 *[Function Name]: idleMode  		     																*
 *[Description]: This function is responsible for enable ECU IDLE mode and only wake up via interrupts  *
 *[In]: void																							*
 *[Out]: void																							*
 ********************************************************************************************************/
void idleMode(void)
{
	SMCR &= ~(1<<SM0) &~(1<<SM1) &~(SM2);
}

/* define makes input via uart only */
#ifdef SERIAL_INPUT

/********************************************************************************************************
 *[Function Name]: addNewMedcine  		     															*
 *[Description]: This function is responsible for adding new medicine to our medicine list via UART.	*
 *				 UART Packet ex==>MedicineName,NumberOfTablets,MedicineStartInHour,						*
 *								  MedicineStartInMinutes,MedicinePeriodicity#							*
 *[In]: void																							*
 *[Out]: void																							*
 ********************************************************************************************************/
void addNewMedcine(void)
{
	/* global medicine counter */
	medCounter++;
	LCD_displayString("new Med");
	_delay_ms(1000);
	LCD_clearScreen();
	LCD_displayString("Enter med Data:");
	uint8 arr[30]; //array to hold uart packet that ends with '#' character
	UART_receiveString(arr);
	uint8 Index = 0;
	/* loop till the next empty medicine using period as indicator to an empty room for next medicine */
	while ((medecineArr[Index].period != 0) && (Index < MAX_MEDECINE_NUM))
	{
		++Index;
	}
	char i = 0;
	char flag = 0;
	char j = 0;
	char temp = 0;
	while(arr[i] != '\0')
	{
		if(arr[i] == ',')
		{
			flag++;
		}
		else if(flag == 0)
		{
			/* medicine name */
			medecineArr[Index].medName[j] = arr[i];
			j++;
		}
		else if(flag == 1)
		{
			/* number of tablets */
			temp = arr[i] - '0';
			medecineArr[Index].tabletsNum = ((medecineArr[Index].tabletsNum) * 10 )+ temp;
		}
		else if(flag == 2)
		{
			/* calculation starting medicine in hours */
			temp = arr[i] - '0';
			medecineArr[Index].start[0] = ((medecineArr[Index].start[0]) * 10 )+ temp;
		}
		else if(flag == 3)
		{
			/* calculation starting medicine in minutes */
			temp = arr[i] - '0';
			medecineArr[Index].start[1] = ((medecineArr[Index].start[1]) * 10 )+ temp;
		}
		else if(flag == 4)
		{
			/* calculation medicine periodicity */
			temp = arr[i] - '0';
			medecineArr[Index].period = (medecineArr[Index].period * 10) + temp;
		}
		else
		{
			/* Packet in receiving UART packet */
			LCD_clearScreen();
			LCD_goToRowColumn(0,0);
			LCD_displayString("ERROR INPUT MED");
			UART_sendString("ERROR INPUT");
			medecineArr[Index].period = 0;
		}
		i++;
	}
	medecineArr[Index].medName[j] = '\0';
	LCD_goToRowColumn(0,0);
	LCD_displayString(medecineArr[Index].medName);
	LCD_goToRowColumn(1,0);
	LCD_intgerToString(medecineArr[Index].tabletsNum);
	LCD_displayCharacter(':');
	LCD_intgerToString(medecineArr[Index].start[1]);
	LCD_displayCharacter(':');
	LCD_intgerToString(medecineArr[Index].start[2]);
	LCD_displayCharacter(':');
	LCD_intgerToString(medecineArr[Index].period);
	_delay_ms(1000);
	LCD_clearScreen();
	if(medCounter == 1)
	{
		/* enable timer module after first medicine added */
		timer_enable();
	}
}
#else

/********************************************************************************************************
 *[Function Name]: addNewMedcine  		     															*
 *[Description]: This function is responsible for adding new medicine to our medicine list via keypad	*
 *				 and using serial for only medicine name.												*
 *[In]: void																							*
 *[Out]: void																							*
 ********************************************************************************************************/
void addNewMedcine(void)
{
	/* global medicine counter */
	medCounter++;
	LCD_displayString("new Med");
	_delay_ms(1000);
	LCD_clearScreen();
	LCD_displayString("Enter med name:");
	uint8 Index = 0;
	/* loop till the next empty medicine using period as indicator to an empty room for next medicine */
	while ((medecineArr[Index].period != 0) && (Index < MAX_MEDECINE_NUM))
	{
		++Index;
	}
	/* medicine name via uart termination with '#' character */
	UART_receiveString(medecineArr[Index].medName);
	LCD_goToRowColumn(0,0);
	LCD_clearScreen();
	LCD_displayString(medecineArr[Index].medName);
	LCD_goToRowColumn(1,0);
	uint8 arr[2];
	for(uint8 i = 0 ; i < 2 ; i++)
	{
		/* input from keypad */
		arr[i] = KeyPad_getPressedKey();
		_delay_ms(500);
	}
	/* number of tablets being calculated */
	medecineArr[Index].tabletsNum = arr[0]*10+arr[1];
	LCD_intgerToString(medecineArr[Index].tabletsNum);
	LCD_displayCharacter(':');
	for(uint8 i = 0 ; i < 2 ; i++)
	{
		arr[i] = KeyPad_getPressedKey();
		_delay_ms(500);
	}
	/* number of hours being calculated */
	medecineArr[Index].start[0] = arr[0]*10+arr[1];
	LCD_intgerToString(medecineArr[Index].start[0]);
	LCD_displayCharacter(':');
	for(uint8 i = 0 ; i < 2 ; i++)
	{
		arr[i] = KeyPad_getPressedKey();
		_delay_ms(500);
	}
	/* number of minutes being calculated */
	medecineArr[Index].start[1] = arr[0]*10+arr[1];
	LCD_intgerToString(medecineArr[Index].start[1]);
	LCD_displayCharacter(':');
	for(uint8 i = 0 ; i < 2 ; i++)
	{
		arr[i] = KeyPad_getPressedKey();
		_delay_ms(500);
	}
	/* medicine periodicity being calculated */
	medecineArr[Index].period = arr[0]*10+arr[1];
	LCD_intgerToString(medecineArr[Index].period);
	_delay_ms(1000);
	LCD_clearScreen();
	if(medCounter == 1)
	{
		/* enable timer module after first medicine added */
		timer_enable();
	}
}
#endif

/********************************************************************************************************
 *[Function Name]: deleteMed  		     																*
 *[Description]: This function is responsible for deleting medicine from our medicine list by making 	*
 *				 medicine periodicity equal to zero.													*
 *[In]: Index of the deleted medicine (unsigned char)													*
 *[Out]: void																							*
 ********************************************************************************************************/
void deleteMed(uint8 medIndx)
{
	if(medecineArr[medIndx].period != 0)
	{
		medecineArr[medIndx].period=0;	
	}
}

/********************************************************************************************************
 *[Function Name]: calcNearestMed  		     															*
 *[Description]: This function is responsible for calculating the nearest medicine in the medicine list *
 *				 by picking up the nearest medicine to the current real time clock.						*
 *[In]: void																							*
 *[Out]: void																							*
 ********************************************************************************************************/
void calcNearestMed(void)
{
	/* get the current time from the RTC */
	uint8 hh,mm,ss;
	RTC_getTime(&hh ,&mm ,&ss );
	/* calculate the temp data in minutes */
	uint32 temp = (medecineArr[0].start[0] - hh)*60 + (medecineArr[0].start[1] - mm); //60 minutes in 1 hour
	nearsestMedIndex_G = 0;
	/* loop on the current available medicines in list */
	for(uint8 index = 1 ; index < medCounter ; index++)
	{
		/* if medicine of current index smaller than temp */
		if((((medecineArr[index].start[0] - hh)*60)+ (medecineArr[index].start[1] - mm)) < temp)
		{
			temp = ((medecineArr[index].start[0] - hh)*60) +(medecineArr[index].start[1] - mm);
			/* update the global index with the minimum one */
			nearsestMedIndex_G = index;
		}
	}
}

/********************************************************************************************************
 *[Function Name]: calcNearestMed  		     															*
 *[Description]: This function is responsible for calculating the nearest medicine in the medicine list *
 *				 by picking up the nearest medicine to the current real time clock.						*
 *[In]: void																							*
 *[Out]: return (unsigned char)																			*
 *								[1]: indicating matching time with RTC, action should be on.			*
 *								[0]: indicating not matching time with RTC.								*
 ********************************************************************************************************/
uint8 medTimeCheck(void)
{
	/* get the current time from the RTC */
	uint8 hh,mm,ss;
	RTC_getTime(&hh, &mm, &ss);
	/* if the current nearest medicine in medicine list equal to current RTC value of hours and minutes */
	if(medecineArr[nearsestMedIndex_G].start[0] == hh && medecineArr[nearsestMedIndex_G].start[1] == mm)
	{
		/* decrease number of tablets */
		medecineArr[nearsestMedIndex_G].tabletsNum-=1;
		/* rescheduler the coming date of this medicine */
		medecineArr[nearsestMedIndex_G].start[0] = (medecineArr[nearsestMedIndex_G].period + medecineArr[nearsestMedIndex_G].start[0]) % 24; //24 day hours		
		if(medecineArr[nearsestMedIndex_G].tabletsNum == 0)
		{
			deleteMed(nearsestMedIndex_G);
		}
		return 1 ; //matching
	}
	return 0 ; //not matching
}