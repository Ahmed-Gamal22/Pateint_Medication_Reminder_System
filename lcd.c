/*
 * lcd.c
 *
 * Created: 20-Apr-20 8:24:46 AM
 * Author: Ahmed Gamal
 */ 
#include "lcd.h"

/********************************************************************************************************
 *[Function Name]: LCD_init				 		     													*
 *[Description]: This function is responsible for initialization LCD hardware module.					*
 *[In]: void																							*
 *[Out]: void																							*
 ********************************************************************************************************/
void LCD_init(void)
{
	/* Configure the control pins(E,RS,RW) as output pins */
	LCD_CTRL_PORT_DIR |= (1<<E) | (1<<RS) | (1<<RW);
	/* Configure the highest 4 bits of the data port as output pins */
	LCD_DATA_PORT_DIR |= 0xF0; 
	/* initialize LCD in 4-bit mode */
	LCD_sendCommand(FOUR_BITS_DATA_MODE);
	/* use 2-line lcd + 4-bit Data Mode + 5*7 dot display Mode */ 
	LCD_sendCommand(TWO_LINE_LCD_Four_BIT_MODE);
	/* cursor off */
	LCD_sendCommand(CURSOR_OFF); 
	/* clear LCD at the beginning */
	LCD_sendCommand(CLEAR_COMMAND);
}

/********************************************************************************************************
 *[Function Name]: LCD_sendCommand				 		     											*
 *[Description]: This function is responsible for sending command to LCD module.						*
 *[In]: unsigned char																					*
 *[Out]: void																							*
 ********************************************************************************************************/
void LCD_sendCommand(uint8 command)
{
	/* Instruction Mode RS=0 */
	CLEAR_BIT(LCD_CTRL_PORT,RS);
	/* write data to LCD so RW=0 */
	CLEAR_BIT(LCD_CTRL_PORT,RW);
	_delay_ms(1); 
	/* Enable LCD E=1 */
	SET_BIT(LCD_CTRL_PORT,E); 
	_delay_ms(1); 
	/* out the highest 4 bits of the required command to the data bus D4 --> D7 */
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (command & 0xF0);
	_delay_ms(1); 
	CLEAR_BIT(LCD_CTRL_PORT,E); 
	_delay_ms(1); 
	SET_BIT(LCD_CTRL_PORT,E); 
	_delay_ms(1); 
	/* out the lowest 4 bits of the required command to the data bus D4 --> D7 */
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | ((command & 0x0F) << 4);
	_delay_ms(1); 
	CLEAR_BIT(LCD_CTRL_PORT,E); 
	_delay_ms(1); 
}

/********************************************************************************************************
 *[Function Name]: LCD_displayCharacter				 		     										*
 *[Description]: This function is responsible for sending byte of data to LCD module.					*
 *[In]: unsigned char																					*
 *[Out]: void																							*
 ********************************************************************************************************/
void LCD_displayCharacter(uint8 data)
{
	/* Instruction Mode RS=0 */
	SET_BIT(LCD_CTRL_PORT,RS);
	/* write data to LCD so RW=0 */
	CLEAR_BIT(LCD_CTRL_PORT,RW);
	_delay_ms(1); 
	/* Enable LCD E=1 */
	SET_BIT(LCD_CTRL_PORT,E); 
	_delay_ms(1); 
	/* out the highest 4 bits of the required command to the data bus D4 --> D7 */
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (data & 0xF0);
	_delay_ms(1); 
	CLEAR_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1); 
	SET_BIT(LCD_CTRL_PORT,E); 
	_delay_ms(1); 
	/* out the lowest 4 bits of the required command to the data bus D4 --> D7 */
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | ((data & 0x0F) << 4);
	_delay_ms(1); 
	CLEAR_BIT(LCD_CTRL_PORT,E); 
	_delay_ms(1); 
}

/********************************************************************************************************
 *[Function Name]: LCD_displayString				 		     										*
 *[Description]: This function is responsible for sending string to LCD module which terminates with'\0'*
 *[In]: unsigned char*																					*
 *[Out]: void																							*
 ********************************************************************************************************/
void LCD_displayString(const char *Str)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		LCD_displayCharacter(Str[i]);
		i++;
	}
}

/********************************************************************************************************
 *[Function Name]: LCD_goToRowColumn				 		     										*
 *[Description]: This function is responsible for moving cursor to new row and col determines by		*
 *				 inputs parameters.																		*
 *[In]: unsigned char , unsigned char																	*
 *[Out]: void																							*
 ********************************************************************************************************/
void LCD_goToRowColumn(uint8 row,uint8 col)
{
	uint8 Address;
	switch(row)
	{
		case 0:
				Address=col;
				break;
		case 1:
				Address=col+0x40;
				break;
	}					
	LCD_sendCommand(Address | SET_CURSOR_LOCATION); 
}

/********************************************************************************************************
 *[Function Name]: LCD_displayStringRowColumn				 	   										*
 *[Description]: This function is responsible for sending string to LCD module which terminates with'\0'*
 *				 at new row and col determines by row and col inputs parameters.						*
 *[In]: unsigned char , unsigned char , unsigned char*													*
 *[Out]: void																							*
 ********************************************************************************************************/
void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str)
{
	LCD_goToRowColumn(row,col);
	LCD_displayString(Str); 
}

/********************************************************************************************************
 *[Function Name]: LCD_intgerToString				 		     										*
 *[Description]: This function is responsible for converted represented data from integer form to string*
 *				 and displayed it on LCD module.														*
 *[In]: unsigned integer																				*
 *[Out]: void																							*
 ********************************************************************************************************/
void LCD_intgerToString(int data)
{
   char buff[16]; 
   itoa(data,buff,10); 
   LCD_displayString(buff);
}

/********************************************************************************************************
 *[Function Name]: LCD_clearScreen				 		     											*
 *[Description]: This function is responsible for removing presented data on LCD hardware module.		*
 *[In]: void																							*
 *[Out]: void																							*
 ********************************************************************************************************/
void LCD_clearScreen(void)
{
	LCD_sendCommand(CLEAR_COMMAND);  
}
