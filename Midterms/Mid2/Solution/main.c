// Assignment: Midterm 2/ Final
// Author : Robert Sander
  
/*
 * ATmega16 Interface with MPU-6050
 * http://www.electronicwings.com
 *
 */ 

#define F_CPU 16000000UL								/* Define CPU clock Frequency e.g. here its 8MHz */
#include <avr/io.h>										/* Include AVR std. library file */
#include <util/delay.h>									/* Include delay header file */
#include <inttypes.h>									/* Include integer type header file */
#include <stdlib.h>										/* Include standard library file */
#include <stdio.h>										/* Include standard library file */
#include "APDS9960_def.h"							/* Include MPU6050 register define file */
#include "I2C_Master_H_file.h"							/* Include I2C Master header file */
#include "USART_RS232_H_file.h"							/* Include USART header file */

#define APDS9960_WRITE 0x72
#define APDS9960_READ 0x73


float clear, red, blue, green;

void APDS9960_Init()										/* Gyro initialization function */
{
	_delay_ms(150);										/* Power up time >100ms */
	//I2C_Start_Wait(APDS9960_WRITE);								/* Start with device write address */
	//I2C_Write(APDS9960_ENABLE);								/* Write to sample rate register */
	//I2C_Write(0x00);									/* 1KHz sample rate */
	//I2C_Stop();
	
	//I2C_Start_Wait(APDS9960_WRITE);								/* Start with device write address */
	//I2C_Write(APDS9960_ENABLE);								/* Write to sample rate register */
	//I2C_Write(0x01);									/* 1KHz sample rate */
	//I2C_Stop();

	I2C_Start_Wait(APDS9960_WRITE);
	I2C_Write(APDS9960_ATIME);								/* Write to power management register */
	I2C_Write(DEFAULT_ATIME);									/* X axis gyroscope reference frequency */
	I2C_Stop();
	

	I2C_Start_Wait(APDS9960_WRITE);
	I2C_Write(APDS9960_CONTROL);									/* Write to Configuration register */
	I2C_Write(DEFAULT_AGAIN);									/* Fs = 8KHz */
	I2C_Stop();

	//I2C_Start_Wait(APDS9960_WRITE);								/* Start with device write address */
	//I2C_Write(APDS9960_ENABLE);								/* Write to sample rate register */
	//I2C_Write(1<<POWER|1<< AMBIENT_LIGHT);									/* 1KHz sample rate */
	//I2C_Stop();
	
	I2C_Start_Wait(APDS9960_WRITE);								/* Start with device write address */
	I2C_Write(APDS9960_ENABLE);								/* Write to sample rate register */
	I2C_Write(0x02);									/* 1KHz sample rate */
	I2C_Stop();

	
}

void APDS_Start_Loc()
{
	I2C_Start_Wait(APDS9960_WRITE);								/* I2C start with device write address */
	I2C_Write(APDS9960_RDATAH);							/* Write start location address from where to read */ 
	I2C_Repeated_Start(APDS9960_READ);							/* I2C start with device read address */
}

void Read_RawValue()
{
	APDS_Start_Loc();									/* Read ambient light value */
	clear = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	red = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	blue = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	green = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	I2C_Stop();
}

int main()
{
	char buffer[20], float_[10];
	float C, R, G, B;
	
	I2C_Init();											/* Initialize I2C */
	APDS9960_Init();										/* Initialize MPU6050 */
	USART_Init(9600);									/* Initialize USART with 9600 baud rate */
	
	while(1)
	{
		USART_SendString("cool");
		Read_RawValue();

		C = clear;	
		R = red;
		G = green;
		B = blue;							/* Divide raw value by sensitivity scale factor to get real values */
		
		dtostrf( C, 3, 2, float_ );					/* Take values in buffer to send all parameters over USART */
		sprintf(buffer," CLEAR LIGHT = %s lumens ", float_);
		USART_SendString(buffer);
		
		dtostrf( R, 3, 2, float_ );					/* Take values in buffer to send all parameters over USART */
		sprintf(buffer," RED LIGHT = %s lumens ", float_);
		USART_SendString(buffer);
		
		dtostrf( G, 3, 2, float_ );					/* Take values in buffer to send all parameters over USART */
		sprintf(buffer," GREEN LIGHT = %s lumens ", float_);
		USART_SendString(buffer);
		
		dtostrf( B, 3, 2, float_ );					/* Take values in buffer to send all parameters over USART */
		sprintf(buffer," BLUE LIGHT = %s lumens ", float_);
		USART_SendString(buffer);
		
		USART_SendString("beans!");
		
	}
}






//USART_tx_string("AT+CWJAP=\"ssid\",\"password\"");
//USART_tx_string("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80");
//USART_tx_string("AT+CIPSEND=51");
//USART_tx_string("GET /update?key=KEY&field1=100");
//USART_tx_string("GET /update?key=key&field1=100");