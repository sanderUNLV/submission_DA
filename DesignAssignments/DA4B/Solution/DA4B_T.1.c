// Assignment: DA4B Task 1
// Author : Robert Sander

#define F_CPU 16000000UL	// Frequency of Xplained Mini (16MHz)
#include <avr/io.h>			// Standard AVR Library
#include <stdio.h>			// AVR library containing printf functions
#include <avr/interrupt.h>	// AVR library containing interrupt functions
#include <util/delay.h>		// AVR library containing _delay_ms() function

#define BAUDRATE 9600										// Baudrate in Bits per second (bps)
#define BAUD_PRESCALLER ((F_CPU / (BAUDRATE * 16UL)) - 1)	// Baudrate Prescaler UBRR0H:UBRR0L

//Declaration of our functions
void Timer1_init(void);						// Function to Initialize Timer1 properties
void adc_init(void);						// Function to initialize Analog to Digital Converter
void read_adc(void);						// Function to read temperature received from ADC

void USART_init(void);						// Function to initialize USART
unsigned char USART_receive(void);			// Function to receive Serial data from UDR0
void USART_send(unsigned char data);		// Function to send individual char data into UDR0
void USART_putstring(char* StringPtr);		// Function to break string into individual chars and send

volatile double adc_val;					// Stores ADC Value representing Analog Voltage
char outs[20];								// 'outs[]' used to store integer and float values into array of chars size 20
volatile double volt;						// voltage 0-4.75
volatile double percent;					// percentage of voltage 0-95%
volatile int potOCR;						// corresponding value of voltage out of 255 to later become new OCR value for timer

int main(void) {
	
	DDRB |= 0x3C; //PB5-PB2 as outputs
	DDRC = 0x00;						// All of PC7:0 Inputs (PC0 Input ADC, PC1 Switch)
	PORTC = (1<<DDC1);					// Set pull-up for PC1
	DDRD |= (1 << DDD6)|(1 << DDD7);	// PD6 and PD7 are Outputs (DC Motor)
	PORTD |= (1 << DDD6)|(0 << DDD7);	// PD6 is HIGH, PD7 is LOW (DC Motor)
	Timer1_init();						// Call the Timer1/PWM initialization code
	adc_init();							// Call the ADC initialization code
	USART_init();						// Call the USART initialization code
	USART_putstring("Connected!\r\n");	// Pass 'Connected!' to function to send serial of chars
	//_delay_ms(125);						// Wait a bit
	OCR1A = potOCR;	

	while (1) {											// Infinite loop
		read_adc();							// Read value of ADC Value
		snprintf(outs,sizeof(outs),"%.2f", volt);	// Stores integer 'volt' into the string 'outs'
		USART_putstring(outs);							// Displays the voltage out of 5V on serial monitor
		USART_putstring(" \n");
		snprintf(outs,sizeof(outs),"%.1f", percent);	// Stores integer 'percent' into the string 'outs'
		USART_putstring(outs);							// Displays the percentage out of 5V on serial monitor
		USART_putstring("%\r\n");
		snprintf(outs,sizeof(outs),"%d", potOCR);	// Stores integer 'potOCR' into the string 'outs'
		USART_putstring(outs);							// Displays the corresponding value for new OCR on serial monitor
		//_delay_ms(100);
		
		TCNT1 = 0;
		while(TCNT1 != OCR1A);
		PORTB = 0x0C; // PB.5 0000 0110 ---> 0000 1100 0x0C
		TIFR1 |= (1 << OCF1A);
		TCNT1 = 0;
		while(TCNT1 != OCR1A);
		PORTB = 0x18; // PB.4 0000 1100 ---> 0001 1000 0x18
		TIFR1 |= (1 << OCF1A);
		TCNT1 = 0;
		while(TCNT1 != OCR1A);
		PORTB = 0x30; // PB.3 0000 1001 ---> 0011 0000 0x30
		TIFR1 |= (1 << OCF1A);
		TCNT1 = 0;
		while(TCNT1 != OCR1A);
		PORTB = 0x24; // PB.2 0000 0110 ---> 0010 0100 0x24
		TIFR1 |= (1 << OCF1A);
		
	}
	return 0;
}
//-----------------------------------------------------------------------------------------------------------
void Timer1_init(void) {					// Function to Initialize Timer0 properties
	TCCR1B |= (1 << WGM12 | 0 << CS12 | 1 << CS10); // CTC MODE WITH OCR1A AS TOP, 0 PRESCALER WITH CS12=256
}
//-----------------------------------------------------------------------------------------------------------
void USART_init(void) {							// Function to Initialize USART properties
	UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);	// Store Upper Baudrate values into UBRR0H
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);		// Store Lower Baudrate values into UBRR0L
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);		// Enable Receiver and Enable Transmitter
	UCSR0C = (3 << UCSZ00);						// Set UCSZ02:1 as 8-bit character data
}
//-----------------------------------------------------------------------------------------------------------
unsigned char USART_receive(void) {				// Function to receive ASCII value from UDR0
	while (!(UCSR0A & (1 << RXC0)));			// Keep Checking until RXC0 is 'High' to break loop
	return UDR0;								// Return received serial into unsigned char data
}
//-----------------------------------------------------------------------------------------------------------
void USART_send(unsigned char data) {			// Function to transmit ASCII value into UDR0
	while (!(UCSR0A & (1 << UDRE0)));			// Keep Checking until  UDRE0 data register 'High' to break loop
	UDR0 = data;								// Store unsigned char serial data into UDR0
}
//-----------------------------------------------------------------------------------------------------------
void USART_putstring(char* StringPtr) {			// Function to break string into chars, then USART_send()
	while (*StringPtr != 0x00) {				// Keep Looping until String Completed (null/0-bits)
		USART_send(*StringPtr);					// Send the unsigned char value pointed by the string pointer
		StringPtr++;							// Increment pointer to next char array location
	}
}
//-----------------------------------------------------------------------------------------------------------
void adc_init(void) {
	DIDR0 = 0x3F;							// Disable Digital Input
	ADMUX = (1<<REFS1)|(1<<REFS0)|			// Reference Selection Bits, AVcc - External cap at AREF
	(0<<ADLAR)|								// ADC Left Adjust Result for 10-bit result
	(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0);// Analog Channel Selection Bits 'ADC0' (PC0)
	ADCSRA = (1<<ADEN)|						// ADC Enable
	(0<<ADSC)|								// ADC Start Conversion
	(0<<ADATE)|								// ADC Auto Trigger Enable
	(0<<ADIF)|								// ADC Interrupt Flag
	(0<<ADIE)|								// ADC Interrupt Enable
	(1<<ADPS2)|(0<<ADPS1)|(1<<ADPS0);		// ADC Prescaler Select Bits '32'
}
//-----------------------------------------------------------------------------------------------------------
void read_adc(void) {
	unsigned char i = 4;				// Set 'i' for iterations
	adc_val = 0;						// set float 'adc_val'
	while (i--) {						// Decrement 'i' until 4 samples take
		ADCSRA |= (1<<ADSC);			// If ADSC is high (ADC Start Conversion)...
		while (ADCSRA & (1<<ADSC));		// Start the ADC Conversion
		adc_val += ADC;					// Store the analog value on of current adc_val
		_delay_ms(100);					// delay 50ms for sampling
	}
	adc_val = (adc_val/4);				// Average of 4 samples taken into adc_val
	//adc_val = (adc_val - 512)/2;		// Lets 10-bit Analog voltage become 8-bit value
	volt = (adc_val*4.75)/1023.0;
	percent = (volt/5.00)*100;
	potOCR = (percent*40000)/100;
}