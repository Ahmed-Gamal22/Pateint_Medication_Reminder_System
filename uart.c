/*
 * uart.c
 *
 * Created: 18-Apr-20 2:27:27 PM
 * Author: Ahmed Gamal
 */
#include "uart.h"

/********************************************************************************************************
 *[Function Name]: UART_init  		     																*
 *[Description]: This function is responsible for initialization UART module.							*
 *[In]: void																							*
 *[Out]: void																							*
 ********************************************************************************************************/
void UART_init(void)
{
	/* U2X = 1 for double transmission speed */
	UCSRA = (1<<U2X);
	/* RXEN  = 1 Receiver Enable, RXEN  = 1 Transmitter Enable */
	UCSRB = (1<<RXEN) | (1<<TXEN);
	/* URSEL   = 1 The URSEL must be one when writing the UCSRC 
	   UCSZ1:0 = 11 For 8-bit data mode */
	UCSRC = (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);
	/* BaudRate = 9600 bit/s */
	UBRRH = 0;
	UBRRL = 154;
}

/********************************************************************************************************
 *[Function Name]: UART_sendByte  		     															*
 *[Description]: This function is responsible for sending data to UART module.							*
 *[In]: unsigned char																					*
 *[Out]: void																							*
 ********************************************************************************************************/	
void UART_sendByte(const uint8 data)
{
	/* wait until UDRE flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,UDRE));
	/* Put the required data in the UDR register */
	UDR = data;
}

/********************************************************************************************************
 *[Function Name]: UART_recieveByte  		     														*
 *[Description]: This function is responsible for receiving data from UART module.						*
 *[In]: void																							*
 *[Out]: unsigned char																					*
 ********************************************************************************************************/
uint8 UART_recieveByte(void)
{
	/* wait until RXC flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC));
	/* Read the received data from the UDR register */
	return UDR;		
}

/********************************************************************************************************
 *[Function Name]: UART_sendString  		     														*
 *[Description]: This function is responsible for sending string to UART module.						*	
 *[In]: umsigmed char*																					*
 *[Out]: void																							*
 ********************************************************************************************************/
void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
}

/********************************************************************************************************
 *[Function Name]: UART_receiveString  	   																*
 *[Description]: This function is responsible for receiving string from UART module which terminated	*
 *				 with '#'.																				*
 *[In]: unsigned char*																					*
 *[Out]: void																							*
 ********************************************************************************************************/
void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;
	Str[i] = UART_recieveByte();
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}
	Str[i] = '\0';
}
