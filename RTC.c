/*
 * RTC.c
 *
 * Created: 18-Apr-20 2:27:27 PM
 * Author: Ahmed Gamal
 */
#include "RTC.h"
#include "i2C.h"

/********************************************************************************************************
 *[Function Name]: DecToBcdConvert  		     														*
 *[Description]: This function is responsible for converting decimal value to BCD one.					*
 *[In]: unsigned char																					*
 *[Out]: unsigned char																					*
 ********************************************************************************************************/
static uint8 DecToBcdConvert(uint8  val)
{
	return val + 6 * (val / 10);
}

/********************************************************************************************************
 *[Function Name]: BcdToDecConvert  		     														*
 *[Description]: This function is responsible for converting BCD value to decimal one.					*
 *[In]: unsigned char																					*
 *[Out]: unsigned char																					*
 ********************************************************************************************************/
static uint8 BcdToDecConvert(uint8  val)
{
	return val - 6 * (val >> 4);
}

/********************************************************************************************************
 *[Function Name]: RTC_init  		     																*
 *[Description]: This function is responsible for initialization RTC by initializing I2C module and then*
 *				 write 0 in RTC control register.														*
 *[In]: void																							*
 *[Out]: void																							*
 ********************************************************************************************************/
void RTC_init()
{
	uint8 Control = 0;
	TWI_init();
	TWI_writeMul(RTC_ADDR , RTC_CONTROL_REG , &Control , sizeof(Control));
	_delay_ms(10);
}

/********************************************************************************************************
 *[Function Name]: RTC_setTime  	    																*
 *[Description]: This function is responsible for setting sequence of data in RTC registers for hours,	*
 *				 minutes, seconds after converting from decimal to BCD.									*
 *[In]: unsigned char hour,unsigned char minute,unsigned char second									*
 *[Out]: void																							*
 ********************************************************************************************************/
void RTC_setTime(uint8 hh ,uint8 mm ,uint8 ss )
{
	uint8 RTC_data[TIMER_ARR_SIZE];
	RTC_data[SECONDS] = DecToBcdConvert(ss);
	RTC_data[MINUTES] = DecToBcdConvert(mm);
	RTC_data[HOURS] = DecToBcdConvert(hh);
	TWI_writeMul(RTC_ADDR , RTC_SECOND_REG , RTC_data , sizeof(RTC_data));
}

/********************************************************************************************************
 *[Function Name]: RTC_setDate  	    																*
 *[Description]: This function is responsible for setting sequence of data in RTC registers for days,	*
 *				 months, years after converting from decimal to BCD.									*
 *[In]: unsigned char day,unsigned char month,unsigned char year										*
 *[Out]: void																							*
 ********************************************************************************************************/
void RTC_setDate(uint8 d ,uint8 m ,uint8 y )
{
	uint8 RTC_data[TIMER_ARR_SIZE];
	RTC_data[SECONDS] = DecToBcdConvert(d);
	RTC_data[MINUTES] = DecToBcdConvert(m);
	RTC_data[HOURS] = DecToBcdConvert(y);
	TWI_writeMul(RTC_ADDR , RTC_DAY_REG , RTC_data , sizeof(RTC_data));
}

/********************************************************************************************************
 *[Function Name]: RTC_getTime  	    																*
 *[Description]: This function is responsible for getting sequence of data from RTC registers for hours,*
 *				 minutes, seconds after converting from BCD to decimal.									*
 *[In]: unsigned char* hour,unsigned char* minute,unsigned char* second									*
 *[Out]: void																							*
 ********************************************************************************************************/
void RTC_getTime(uint8 *hh ,uint8 *mm ,uint8 *ss )
{
	uint8 RTC_data[TIMER_ARR_SIZE];
	TWI_readMul(RTC_ADDR , RTC_SECOND_REG , RTC_data , sizeof(RTC_data));
	*ss = BcdToDecConvert(RTC_data[SECONDS]);
	*mm = BcdToDecConvert(RTC_data[MINUTES]);
	*hh = BcdToDecConvert(RTC_data[HOURS]);
}

/********************************************************************************************************
 *[Function Name]: RTC_getDate  	    																*
 *[Description]: This function is responsible for getting sequence of data from RTC registers for days,	*
 *				 months, years after converting from BCD to decimal.									*
 *[In]: unsigned char* day,unsigned char* month,unsigned char* year										*
 *[Out]: void																							*
 ********************************************************************************************************/
void RTC_getDate(uint8 *d ,uint8 *m ,uint8 *y )
{
	uint8 RTC_data[TIMER_ARR_SIZE];
	TWI_readMul(RTC_ADDR , RTC_DAY_REG , RTC_data , sizeof(RTC_data));
	*d = BcdToDecConvert(RTC_data[SECONDS]);
	*m = BcdToDecConvert(RTC_data[MINUTES]);
	*y = BcdToDecConvert(RTC_data[HOURS]);
}