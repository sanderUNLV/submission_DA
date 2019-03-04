// DA2A_T2_C
// Author : Robert Sander

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main (void)
{
	/* SET PORTB FOR OUTPUT*/
	DDRB |= 0xFF;  //ALL BITS AS INPUTS TO TURN OFF THE BITS IN PINB
	PORTB |= 0x3C; // TURN OFF LED PB.2-5
	/* SET PORTC FOR INPUT*/     //PC.0 IS CONNECTED TO POTENTIOMETER
	DDRC |= (1<<2);    //SETS PC.2 AS AN INPUT
	PORTC |= (1 << 2); // ENABLES PULL-UP IN PC.2, PUTS SWITCH IN TO "OFF POSITION"
	while (1) {
		if (!(PINC & (1<<PINC2))) //CONSTANTLY POLL TO SEE CONDITION FOR PINC, CHECK TO SEE IF THE PIN IS HIGH, BUTTON IS PUSHED
		{
			PORTB &= ~(1<<2);   // SET PB.2 TO 0 TO TURN THE LED ON, REVERSE LOGIC
			_delay_ms(1250);	//DELAY 1.25s
		}
		else
			PORTB |= (1<<2);    // SET PB.2 TO 1 TO TURN OFF THE LED
	}
	return 0;
}



