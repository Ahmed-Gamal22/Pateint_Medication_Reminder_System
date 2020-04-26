/*
 * i2c.h
 *
 * Created: 19-Apr-20 1:26:11 PM
 * Author: Ahmed Gamal
 */ 
/************************************************************************ 
 *				 Two wire interface module                              * 
 ************************************************************************/
#ifndef I2C_H_
#define I2C_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"
enum
{
	WITHACK,
	WITHOUTACK,
};
enum
{
	I2C_WRITE,
	I2C_READ
};
/******************************************************************************* 
 *                      Functions Prototypes                                   * 
 *******************************************************************************/
void TWI_init(void);
void TWI_writeMul(uint8 slaveAddress , uint8 regAddress , uint8 *data , uint8 length);
void TWI_readMul(uint8 slaveAddress , uint8 regAddress , uint8 *data , uint8 length);
#endif 
