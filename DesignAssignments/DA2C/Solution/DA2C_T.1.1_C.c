// DA2C_T.1.1_C
// Author : Robert Sander

#include <avr/io.h>

volatile int counter = 0;

int main(void)
{
	DDRB = 0b00111100; //PB2-5 as output
	PORTB = _BV(PINB2)|_BV(PINB3)|_BV(PINB4)|_BV(PINB5); // TURN OFF ALL LEDS AT FIRST
	TCCR0A = 0; // Normal Operation
	TCCR0B |= (1<<CS01)|(1<<CS00); // set prescaler to 64 and start the timer
	TCNT0=0x00; // start the timer

	while (1)
	{
		// wait for the overflow event
		while ((TIFR0 & 0x01) == 0);
		{
			counter++; // Increment counter, TCNT0 goes back to 0 when the overflow flag is set
			if (counter >= 425) // (((435ms/0.0000625ms)/64)/256)=424.8, this is how many times we have to count the overflow for 435ms
			{
				PORTB = ~(1 << 2); // Turn on LED
			}
			if (counter >= 708) // (((290ms/0.0000625ms)/64)/256)=283.2, this is how many times we have to count the overflow for 290ms
			{
				PORTB = 0x3C; // Turn off LED
				counter = 0; // RESET COUNTER
			}
			TIFR0=0x01;	// reset the overflow flag
			TCNT0 = 0;	// reset timer
		}
	}
}

