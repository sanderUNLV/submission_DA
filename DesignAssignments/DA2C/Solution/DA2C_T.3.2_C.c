// DA2C_T.3.2_C
// Author : Robert Sander

#include <avr/io.h>
#include <avr/interrupt.h>

/*
 Needs to be global in scope so that the intterupt service routine has access to it.
 It cannot be passed to the routine. It also must have the volatile modifier because
 the compiler has no idea when the ISR will activate and change the variable.
 This prevents the complier from making assumptions as to the value of the variable.
*/
volatile int counter = 0;	

int main ()
{
	DDRB = 0b00111100; //PB2-5 as output
	PORTB = 0x3C;	// Turn off LED PB.2-5
	OCR0A = 255;	//this value is compared with TCNT0, a match generates an output compare interrupt
	TCCR0A |= (1<<WGM01);	//CTC mode, internal clk, TOP=OCR0A
	TCCR0B |= (1<<CS02|0<<CS01|0<<CS00);	// 256 prescaler
	TIMSK0 = (1<<OCIE0A);	//enable Timer0 compare match int.
	sei ();			//enable interrupts
	TCNT0 = 0; // Initial value
	
	while (1)		//wait here
	{
		// Stay here for indefinitely
	}
}

ISR (TIMER0_COMPA_vect)		//ISR for Timer0 compare match
{
	counter++;	// Increment the interrupt counter by 1
	if (!(PINC & (1<<PINC2))) //CONSTANTLY POLL TO SEE CONDITION FOR PINC
	{
		counter = 0;
		TCNT0 = 0;
		PORTB = ~(1 << 2);	// Turn on LED PB.2
	}
	if (counter >= 280)
	{
		PORTB = 0x3C;	// Turn off LED PB.2-5
		counter = 0;	// Reset counter
	}
}


