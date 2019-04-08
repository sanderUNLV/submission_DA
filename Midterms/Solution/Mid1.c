// Mid1.c
// Author : Robert Sander

#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

//Declaration of our functions
void USART_init( unsigned int ubrr);
void USART_tx_string( char *data );
void adc_init(void);
void read_adc(void);
void esp_init();
void USART_putadc();

volatile double adc_temp = 0;
char outadc[20];

int main(void) {

	DDRB = 0x3C;	// Set PB2.5 as input
	PORTB = 0x3C;	// Turn off PB LEDs on multishield
	DDRC = 0;	// Make Port C an input for ADC input
	adc_init();	// Initialize the ADC
	USART_init(BAUD_PRESCALLER);	// Call the USART initialization code
	esp_init(); // Initialize the esp with AT commands

	while (1){
		
		esp_init();
		USART_tx_string("Temperature: ");
		USART_putadc();
		_delay_ms(17000); // Delay serial data for at least 15 seconds for thingspeak minimum
	}
}

void adc_init(void)
{
	// Setup and enable ADC
	ADMUX = (0<<REFS1)|    // Reference Selection Bits
	(1<<REFS0)|  // AVcc with ext cap at AREF pin
	(0<<ADLAR)| // ADC Left Adjust Result
	(0<<MUX3)|	// Analog Channel Selection Bits
	(1<<MUX2)|
	(0<<MUX1)|  
	(1<<MUX0);	// ADC5
	ADCSRA = (1<<ADEN)|    // ADC ENable
	(1<<ADSC)|     // ADC Start Conversion
	(1<<ADATE)|    // ADC Auto Trigger Enable
	(0<<ADIF)|     // No ADC Interrupt Flag
	(0<<ADIE)|     // No ADC Interrupt Enabled
	(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // ADC Prescaler - 128
}

void esp_init(){
	
	USART_tx_string("AT+CWJAP=\"ssid\",\"password\"");
	USART_tx_string("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80");
	USART_tx_string("AT+CIPSEND=51");
	USART_tx_string("GET /update?key=KEY&field1=100");
	USART_tx_string("GET /update?key=key&field1=100");	
}	

void read_adc(void)
{
	unsigned char i = 4;
	while (i--)
	{
		while((ADCSRA&(1<<ADIF))==0);//wait for conversion to finish
		PORTB ^= (1 << 2); // Toggle PB.2 LED every time ADIF is set, indicated adc conversion visually
		adc_temp += ADC; // Put adc value in "adc_temp"
		_delay_ms(100);	// Wait a 100ms
		ADCSRA |= (1<<ADIF); // Clear the flag
	}
	adc_temp = adc_temp / 4;  // Average a 4 samples
}

void USART_init( unsigned int ubrr) {

	UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1 << TXEN0);
	UCSR0C = (3 << UCSZ00);
}

void USART_tx_string( char *data )
{
	while ((*data != '\0'))
	{
		while (!(UCSR0A & (1 <<UDRE0)));
		UDR0 = *data;
		data++;
	}
}

void USART_putadc(){
	read_adc(); // initialize adc reading
	snprintf(outadc, sizeof(outadc), "%3f\r\n", adc_temp); // print it
	USART_tx_string(outadc);
}











