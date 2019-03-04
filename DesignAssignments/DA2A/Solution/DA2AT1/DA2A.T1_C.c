// DA2A_T1_C
// Author : Robert Sander

#define F_CPU 16000000UL

#include <avr/io.h>     //include this input output library
#include <util/delay.h> //include this delay library

int main (void)
{
	DDRB |= 0x3C;        //SETS PB.2-5 FOR OUTPUT
	while (1) {
		PORTB = ~(1<<2); //&= SET IT TO ZERO...TURNS THE LED ON...REVERSE LOGIC
		_delay_ms(290);  //0.725 x 0.4=0.290s
		PORTB = 0x3C;    // PUT 1'S IN ALL 8 BITS WHICH TURNS OFF THE LEDS 0011 1100 ONLY TURNS OFF PB.2-5
		_delay_ms(435);  //0.725 x 0.6=0.435s - 60% DUTY CYCLE ON
	}
	return 0;
}
