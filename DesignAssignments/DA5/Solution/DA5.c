// 	MIT License  Geo Vanni Portillo Rocky Gonzalez
// 
// 	Copyright (c) 2018 Helvijs Adams
// 
// 	Permission is hereby granted, free of charge, to any person obtaining a copy
// 	of this software and associated documentation files (the "Software"), to deal
// 	in the Software without restriction, including without limitation the rights
// 	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// 	copies of the Software, and to permit persons to whom the Software is
// 	furnished to do so, subject to the following conditions:
// 
// 	The above copyright notice and this permission notice shall be included in all
// 	copies or substantial portions of the Software.
// 
// 	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// 	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// 	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// 	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// 	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// 	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// 	SOFTWARE.

//
//	Software was tested on ATmega328P and ATmega328PB (PB needs few changes in SPI)
//	RF module software was tested on - cheap nRF24L01+ from China
//	All the relevant settings are defined in nrf24l01.c file
//	Some features will be added later, at this moment it is bare minimum to send/receive
//

//	Set clock frequency
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//	Set up UART for printf();
#ifndef BAUD
#define BAUD 9600
#endif
#include "STDIO_UART.h"

//	Include nRF24L01+ library
#include "nrf24l01.h"
#include "nrf24l01-mnemonics.h"
#include "spi.h"
void print_config(void);

void adc_init(void);
void read_adc(void);

volatile double adc_temp = 0;

//	Used in IRQ ISR
volatile bool message_received = false;
volatile bool status = false;

int main(void)
{	
	adc_init(); // Initialize the ADC	
	//	Set cliche message to send (message cannot exceed 32 characters)
	char tx_message[32];				// Define string array
	strcpy(tx_message,"Hello World!");	// Copy string into array
	//	Initialize UART
	uart_init();
	//	Initialize nRF24L01+ and print configuration info
    nrf24_init();
	print_config();
	_delay_ms(1000);
	//	Start listening to incoming messages
	nrf24_start_listening();
	//status = nrf24_send_message(tx_message);
	//if (status == true) printf("Connection established\n");
	
    while (1) 
    {
		read_adc();
		snprintf(tx_message, sizeof(tx_message), "%3f\r\n", adc_temp); // print it
		status = nrf24_send_message(tx_message);
		if (message_received)
		{
			//	Message received, print it
			message_received = false;
			printf("Received message: %s\n",nrf24_read_message());
			//	Send message as response
			_delay_ms(500);
			status = nrf24_send_message(tx_message);
			if (status == true) printf("Message sent successfully\n");
			//USART_putadc();
		}
    }
}

//	Interrupt on IRQ pin
ISR(INT0_vect) 
{
	message_received = true;
}

void print_config(void)
{
	uint8_t data;
	printf("Startup successful\n\n nRF24L01+ configured as:\n");
	printf("-------------------------------------------\n");
	nrf24_read(CONFIG,&data,1);
	printf("CONFIG		0x%x\n",data);
	nrf24_read(EN_AA,&data,1);
	printf("EN_AA			0x%x\n",data);
	nrf24_read(EN_RXADDR,&data,1);
	printf("EN_RXADDR		0x%x\n",data);
	nrf24_read(SETUP_RETR,&data,1);
	printf("SETUP_RETR		0x%x\n",data);
	nrf24_read(RF_CH,&data,1);
	printf("RF_CH			0x%x\n",data);
	nrf24_read(RF_SETUP,&data,1);
	printf("RF_SETUP		0x%x\n",data);
	nrf24_read(STATUS,&data,1);
	printf("STATUS		0x%x\n",data);
	nrf24_read(FEATURE,&data,1);
	printf("FEATURE		0x%x\n",data);
	printf("-------------------------------------------\n\n");
}

void adc_init(void)
{
	// Setup and enable ADC
	ADMUX = (0<<REFS1)|    // Reference Selection Bits
	(1<<REFS0)|  // Internal 1.1
	(0<<ADLAR)| // ADC Left Adjust Result
	(0<<MUX3)|	// Analog Channel Selection Bits
	(1<<MUX2)|
	(0<<MUX1)|  // ADC5
	(0<<MUX0);
	ADCSRA = (1<<ADEN)|    // ADC ENable
	(1<<ADSC)|     // ADC Start Conversion
	(1<<ADATE)|    // ADC Auto Trigger Enable
	(0<<ADIF)|     // ADC Interrupt Flag
	(0<<ADIE)|     // ADC Interrupt Enable
	(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // ADC Prescaler - 128
	
}

void read_adc(void)
{
	unsigned char i = 4;

	while (i--)
	{
		//ADCSRA |= (1<<ADSC);
		while((ADCSRA&(1<<ADIF))==0);//wait for conversion to finish
		PORTB ^= (1 << 2);
		adc_temp += ADC;
		_delay_ms(100);
		//ADCSRA |= (1<<ADIF);
	}
	adc_temp = adc_temp / 4;  // Average a few samples
}
