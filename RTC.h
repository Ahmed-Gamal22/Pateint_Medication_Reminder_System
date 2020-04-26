/*
 * RTC.h
 *
 * Created: 18-Apr-20 2:27:27 PM
 * Author: Ahmed Gamal
 */
#ifndef RTC_H_
#define RTC_H_
#include "common_macros.h"
#include "std_types.h"
#include "micro_config.h"

/* RTC DS1307 Slave Address */
#define RTC_ADDR	0xD0

enum
{
	SECONDS,
	MINUTES,
	HOURS,
	TIMER_ARR_SIZE
};
/* Addresses of register inside RTC */
enum
{
	RTC_SECOND_REG ,
	RTC_MINUTE_REG ,
	RTC_HOUR_REG ,
	RTC_DAY_REG = 0x04,
	RTC_MONTH_REG ,
	RTC_YEAR_REG ,
	RTC_CONTROL_REG ,
};

/******************************************************************************************************* 
 *                      Functions Prototypes							                               * 
 *******************************************************************************************************/
void RTC_init();
void RTC_setTime(uint8 hh ,uint8 mm ,uint8 ss );
void RTC_getTime(uint8 *hh ,uint8 *mm ,uint8 *ss );
void RTC_setDate(uint8 d ,uint8 m ,uint8 y );
void RTC_getDate(uint8 *d ,uint8 *m ,uint8 *y );

#endif
