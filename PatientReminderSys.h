/*
 * PatientReminderSys.h
 *
 * Created: 20-Apr-20 8:24:46 AM
 * Author: Ahmed Gamal
 */ 
#ifndef PATIENTREMINDERSYS_H_
#define PATIENTREMINDERSYS_H_

#include "common_macros.h"
#include "std_types.h"
#include "micro_config.h"
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "i2c.h"
#include "exInterrupt.h"
/* Maximum number of medicine should be determine according to memory size */
#define MAX_MEDECINE_NUM 20
/********************************************************************************************************
 * define serial input will use uart serial terminal as our main input data								*
 * default input interface will be using serial for name only and keypad for other timing inputs		*
 ********************************************************************************************************/
#define SERIAL_INPUT

/********************************************************************************************************
 * Enum to define each state of our finite state machine											    *
 *		enum fields:																				    *
 *					1- Idle state: State at which ECU will be responding only to interrupts             *
 *					2- addMedcine: State at which ECU will wait to enter user medicine input data.	    *
 *					3- nearestMed: State at which ECU will calculate the next nearest medicine in time. *
 *					4- updateTime: State at which ECU will check if the medicine time matching RTC.	    *																			 *
 *					5- takeAction: State at which ECU will make buzzer on and print medicine name on LCD*
 *					6- doNothing:  State at which ECU will do nothing just burn cycles. 				*
 *					7- stopAlert: State at which ECU will stop buzzer and print ALERT OFF.				*
 ********************************************************************************************************/
typedef enum{
	idle,
	addMedcine,
	nearestMed,
	updateTime,
	takeAction,
	doNothing,	
	stopAlert
}state_t;

/*****************************************************************************************************
 * Structure fields:																				 *
 *					1- medName[10]: array of 10 characters used to medicine name.				     *
 *					2- tabletsNum: number of tablets to be taken by the patient.					 *
 *					3- start[2]: consists of two fields of starting medicine time:					 *
 *														start[0]: contains medicine hour.			 *
 *														start[1]: contains medicine minutes.		 *
 *					4- period: defines when the medicine will be taken again.						 *
 *****************************************************************************************************/
typedef struct{
	uint8 medName[10];
	uint8 tabletsNum;
	uint8 start[2];	
	uint8 period;
}medecine_t;

/* IDLE mode Register in AVR32 (not included in AVR\io header) */
#define SMCR (*(volatile uint8*)0x53)   
/* pins used in idle mode */
#define SM0 1
#define SM1 2
#define SM2 3

/******************************************************************************************************* 
 *                      Functions Prototypes							                               * 
 *******************************************************************************************************/
void sysInit(void); 
void idleMode(void);
void addNewMedcine(void);
void calcNearestMed(void);
uint8 medTimeCheck(void);
void action_on(void);
void action_off(void);
void deleteMed(uint8 medIndx);

#endif /* PATIENTREMINDERSYS_H_ */