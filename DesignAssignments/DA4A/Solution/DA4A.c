// DA4A_C
// Author : Robert Sander

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int main (void)
{
	PCMSK1 = (1<<PCINT9); // Set PC.1 as the interrupt pin
	PCICR = (1<<PCIE1); // For PCINT[14:8]
	sei();	// enable interrupts
	DDRC |= (0<<3)|(0<<2)|(0<<1);
	DDRB |= 0xFE;  //ALL BITS AS OUTPUTS EXCEPT PB.0
	PORTB |= 0x3E; // TURN "OFF" LED PB.2-5 0011 1110
	ICR1 = 0xFFFF; // set TOP to 16bit
	TCCR1A |= (1 << COM1A1)|(1 << COM1B1); // set none-inverting mode
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12)|(1 << WGM13);
	// set Fast PWM mode using ICR1 as TOP
	TCCR1B |= (1 << CS10); // START the timer with no prescaler
	
	while(1) //infinite loop
	{
		if(!(PINC & (1<<PINC2))) //If switch PC.2 is pressed
		{
			PORTB = ~(1<<4); // TURN PB.4 LED ON
			OCR1A = 13108;	// 65536*(0.20)=13107.2
			// set PWM for 20% duty cycle @ 16bit
			// keep motor at 20% speed
		}
		if(!(PINC & (1<<PINC3))) //If switch PC.3 is pressed
		{
			PORTB &= (1<<4)|(1<<3)|(1<<2);
			PORTB = ~(1<<5); // TURN PB.5 LED ON
			OCR1A = 62260;	// 65536*(0.95)=62259.2
			// set PWM for 95% duty cycle @ 16bit
			//keep motor at 95% speed
		}
	}
}

ISR(PCINT1_vect) // SET PCINT9 TO STOP THE MOTOR
{
	PORTB = (1<<5)|(1<<4)|(1<<2);
	PORTB &= ~(1<<3);   // TURN PB.2 LED ON
	OCR1A = 0; // stop motor using interrupt
}