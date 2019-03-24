// DA2C_T.2.2_C
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
	TCCR0A = 0; // Normal Operation
	TCCR0B |= (1<<CS02)|(0<<CS01)|(0<<CS00); // 256 prescaler
	TIMSK0 |= (1 << TOIE0);// Enable Timer/Counter0 Overflow Interrupt Enable
	sei(); //enable interrupts
	TCNT0 = 0; // initial value
		
	while (1)
	{
		if (!(PINC & (1<<PINC2))) //CONSTANTLY POLL TO SEE CONDITION FOR PINC, CHECK TO SEE IF THE PIN IS HIGH, BUTTON IS PUSHED
		{
			TCNT0 = 0;		// Reset Timer
			counter = 0;	// Reset interrupt counter
			PORTB = ~(1 << 2);   // SET PB.2 TO 0 TO TURN THE LED ON, REVERSE LOGIC
		}
	}
}
ISR (TIMER0_OVF_vect) // timer0 overflow interrupt
{
	counter++;	// Increment the interrupt counter by 1
	if (counter >= 280)	// This should be 305 but 280 is more accurate for some reason
	{
		PORTB = 0x3C;	// Turn off LED PB.2-5
		counter = 0;	// Reset interrupt counter
	}
}