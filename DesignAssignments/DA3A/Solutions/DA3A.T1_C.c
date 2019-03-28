// DA3A_T.1_C
// Author : Robert Sander

#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

//Declaration of our functions
void USART_init(void);
void USART_send(unsigned char data);

// String
void USART_putstring(char *StringPtr);
char String[] = "Hello! ";  //String[] is in fact an array but when we put the text between the " " symbols the compiler threats it as a String and automatically puts the null termination character in the end of the text

// Random Integer
void USART_putrandomint(char *randint);
volatile int randint = 8;
char randintout[2];


// Floating point number
void USART_putfloating(char *floatingnumber);
volatile double floatingnumber = 0.123;
char fltouts[20];

// Space
void USART_putspace(char *SpacePtr);
char Space[] = " ";

int main(void) {

	OCR1A = 62500;			// 16000000/256=62500 - we set the new TOP number to 62500 for 1s delay
	TCCR1B |= (1<<CS12);	//int_clk internal clk, 256 prescaler
	TCCR1A |= (1<<WGM12);	//CTC mode TOP=OCR1A
	TIMSK1 = (1<<OCIE1A);	//enable Timer1 compare match int.
	sei ();			//enable interrupts
	USART_init();        //Call the USART initialization code

	while (1) 
	{      
		//Infinite loop
	}
}

void USART_init(void) {

	UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1 << TXEN0);
	UCSR0C = (3 << UCSZ00);
}

void USART_send(unsigned char data) {

	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;

}

void USART_putstring(char* StringPtr) 
{

	while (*StringPtr != 0x00) {
		USART_send(*StringPtr);
		StringPtr++;
	}

}

void USART_putrandomint(char *randint)
{

	while (*randint != 0x00) {
		USART_send(*randint);
		randint++;
	}

}

void USART_putfloating(char *floatingnumber) 
	{

	while (*floatingnumber != 0x00) {
		USART_send(*floatingnumber);
		floatingnumber++;
	}

}

void USART_putspace(char* SpacePtr)
{

	while (*SpacePtr != 0x00) {
		USART_send(*SpacePtr);
		SpacePtr++;
	}

}

ISR (TIMER1_COMPA_vect)		//ISR for Timer0 compare match
{
	
	USART_putstring(String);    //Pass the string to the USART_putstring function and sends it over the serial
	snprintf(randintout,sizeof(randintout),"%d\r\n", randint);	// print random integer
	USART_putrandomint(randintout);
	USART_putstring(Space);
	snprintf(fltouts,sizeof(fltouts),"%f\r\n", floatingnumber);  // print floating point number
	USART_putfloating(fltouts);

	TIFR0 |= (1<<OCF0A); // clear the compare A match flag
}