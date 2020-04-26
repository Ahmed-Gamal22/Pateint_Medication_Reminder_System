/*
 * i2c.c
 *
 * Created: 19-Apr-20 1:26:11 PM
 * Author: Ahmed Gamal
 */ 
#include "i2c.h"

/**********************************************************************
 *                     Static Functions Prototypes                    *
 *********************************************************************/
static void TWI_start(void);
static void TWI_stop(void);
static void TWI_write(uint8 data);
static uint8 TWI_read(uint8 ack); //Read Date With Send ACK Or NACK

/*********************************************************************
 *[Function Name]: TWI_init  		     							 *
 *[Description]: This function is responsible for initialization     *
 * 				 I2C module for the first time(F_CPU=12MHz,50Kbps).  *
 *[In]: void														 *
 *[Out]: void														 *
 ********************************************************************/
void TWI_init(void)
{
    TWBR = 0x70;
	TWSR = 0x00;
    TWAR = 0b00000010; 
    TWCR = (1<<TWEN); 
}

/*********************************************************************
 *[Function Name]: TWI_start  		     							 *
 *[Description]: This function is responsible for sending start bit  *
 * 				 to SDA line.										 *
 *[In]: void														 *
 *[Out]: void														 *
 ********************************************************************/
void TWI_start(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while(BIT_IS_CLEAR(TWCR,TWINT));
}

/*********************************************************************
 *[Function Name]: TWI_stop  		     							 *
 *[Description]: This function is responsible for sending stop bit   *
 * 				 to SDA line and terminate communication.			 *
 *[In]: void														 *
 *[Out]: void														 *
 ********************************************************************/
void TWI_stop(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

/*********************************************************************
 *[Function Name]: TWI_write  		     							 *
 *[Description]: This function is responsible for writing data byte  *
 * 				 to SDA line.										 *
 *[In]: uint8 data (unsigned char)									 *
 *[Out]: void														 *
 ********************************************************************/
void TWI_write(uint8 data)
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while(BIT_IS_CLEAR(TWCR,TWINT));
}

/*********************************************************************
 *[Function Name]: TWI_read 		     							 *
 *[Description]: This function is responsible for reading one byte   *
 * 				 from data bus with or without acknowledgment.		 *
 *[In]: uint8 ack (unsigned char)									 *
 *[Out]: uint8 data (unsigned char)									 *
 ********************************************************************/
uint8 TWI_read(uint8 ack)
{
	switch(ack)
	{
		case WITHACK:
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
		break;
		case WITHOUTACK:
		TWCR = (1 << TWINT) | (1 << TWEN);
		break;
	}
	while(BIT_IS_CLEAR(TWCR,TWINT));
	return TWDR;
}

/*********************************************************************
 *[Function Name]: TWI_writeMul 		     						 *
 *[Description]: This function is responsible for writing one or more*
 * 				  byte/s to data bus by looping on data length.		 *
 *[In]: -Slave Address		(unsigned char)							 *
 *		-Register Address	(unsigned char)							 *
 *		-Data				(unsigned char*)						 *
 *		-length				(unsigned char)							 *
 *[Out]: void														 *
 ********************************************************************/
void TWI_writeMul(uint8 slaveAddress , uint8 regAddress , uint8 *data , uint8 length)
{
	TWI_start();
	TWI_write(slaveAddress | I2C_WRITE);
	TWI_write(regAddress);
	for(int index = 0; index < length ;index++)
	{
		TWI_write(data[index]);
	}
	TWI_stop();
}

/*********************************************************************
 *[Function Name]: TWI_readMul 		     							 *
 *[Description]: This function is responsible for reading one or more*
 * 				  byte/s to data bus by looping on data length.		 *
 *[In]: -Slave Address		(unsigned char)							 *
 *		-Register Address	(unsigned char)							 *
 *		-Data				(unsigned char*)						 *
 *		-length				(unsigned char)							 *
 *[Out]: void														 *
 ********************************************************************/
void TWI_readMul(uint8 slaveAddress , uint8 regAddress , uint8 *data , uint8 length)
{
	TWI_start();
	TWI_write(slaveAddress | I2C_WRITE);
	TWI_write(regAddress);
	TWI_stop();
	TWI_start();
	TWI_write(slaveAddress | I2C_READ);
	int index = 0;
	if(length == 1)
	{
		*data = TWI_read(WITHOUTACK);
	}
	else
	{
		for(index = 0 ; index < length-1 ; index ++)
		{
			data[index] = TWI_read(WITHACK);
		}
		data[index] = TWI_read(WITHOUTACK);
	}
	TWI_stop();
}