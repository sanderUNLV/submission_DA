// DA2C_T.2.1_C
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

int main()
{
	DDRB = 0b00111100; // PB2-5 as output
	PORTB = 0x3C;	// Turn off LED PB.2-5
	TIMSK0 |= (1 << TOIE0);// Enable Timer/Counter0 Overflow Interrupt Enable
	TCCR0A = 0; // Normal Operatio
	TCCR0B |= (1 << CS01) | (1 << CS00); // 64 prescaler
	sei(); //enable interrupts
	TCNT0 =0x00; // initial value
	
	while (1)
	{
		//STAY HERE INDEFINITELY			
	}
}
ISR (TIMER0_OVF_vect) // timer0 overflow interrupt
{
	counter++;	// Increment the interrupt counter by 1
	if (counter >= 425)
	{
		PORTB = ~(1 << 2);	// Turn on LED PB.2
		if (counter >= 708)
		{
			PORTB = 0x3C;	// Turn off LED PB.2-5
			counter = 0;	// Reset counter
		}
	}
}