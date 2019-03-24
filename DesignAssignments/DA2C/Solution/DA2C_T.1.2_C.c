// DA2C_T.1.2_C
// Author : Robert Sander

#include <avr/io.h>

volatile int counter = 0;

int main() // what does having/not having "void" in here do?
{
	DDRB = 0b00111100; //PB2-5 as output
	PORTB = _BV(PINB2)|_BV(PINB3)|_BV(PINB4)|_BV(PINB5); // TURN OFF ALL LEDS AT FIRST
	// SET PORTC FOR INPUT     //PC.0 IS CONNECTED TO POTENTIOMETER
	TCCR0A = 0; // Normal Operation
	TCCR0B |= (1<<CS02)|(0<<CS01)|(0<<CS00); // set prescaler to 256 and start the timer
	TCNT0 = 0x00; // start the timer

	while (1)
	{
		if (!(PINC & (1 << PINC2))) // Polling for PC.2
		{	
			TCNT0= 0x00; // reset timer
			counter = 0;
			PORTB = ~(1 << 2); // Turn on LED PB.2
		}
		// wait for the overflow event
		while ((TIFR0 & 0x01) == 0); // If overflow flag is set, break out
		{
			counter++; // Increment counter, TCNT0 goes back to 0 when the overflow flag is set
			if (counter >= 280) // (((1250ms/0.0000625ms)/256)/256)=305.2, this is how many times we have to count the overflow for 290ms
			{
				PORTB = 0x3C; // Turn off LED
				counter = 0; // RESET COUNTER
			}
			TIFR0 = 0x01;	// reset the overflow flag
		}
		
	}
}
