// DA2B_T1_C
// Author : Robert Sander

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main ()
{
		DDRB |= 0xFF;// ENABLE PORTB AS OUTPUT
		PORTB |= 0x3C;// TURN OFF PB.2-5
		PORTD |= 1<<2;//pull-up activated FOR INT0
		EICRA = 0x2;//make INT0 falling edge triggered
		EIMSK = (1<<INT0);//enable external interrupt 0 PD.2
		sei ();//enable interrupts, global

	while (1)
	{
		// DO WHATEVER HERE, LOOPING INDEFINITELY
	}
	return 0;
}

ISR (INT0_vect)//ISR for external interrupt 0
{
	PORTB ^= (1<<2); // TURN ON PB.2	
	_delay_ms(1250); // DELAY 1.25s
}



