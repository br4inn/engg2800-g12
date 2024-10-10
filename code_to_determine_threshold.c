#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define F_CPU 8000000UL    // Assume a clock of 16 MHz
#define BAUD 9600
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)

#define PHOTODIODE PC0       // Photodiode connected to PC0

void USART_init() {
	// Set baud rate
	UBRR0H = (BAUD_PRESCALE >> 8);
	UBRR0L = BAUD_PRESCALE;
	
	// Enable transmitter
	UCSR0B = (1 << TXEN0);
	
	// Set frame format: 8 data bits, 1 stop bit
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
}

// Send a single character over USART
void USART_send_char(char c) {
	// Wait for the transmit buffer to be ready
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;  // Load the data into the buffer
}


void USART_send_string(const char* str) {
	while (*str) {
		USART_send_char(*str++);
	}
}

void ADC_init() {
	// Set up ADC
	ADMUX = (1 << REFS0);  // AVcc as reference, ADC0 (PC0) as input
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); // Enable ADC, prescaler of 64
}

uint16_t ADC_read() {
	ADCSRA |= (1 << ADSC); // Start conversion
	while (ADCSRA & (1 << ADSC)); // Wait until conversion is complete
	return ADC;
}



int main(void) {
	char buffer[16];    // Buffer to hold the string to send via USART
	
	ADC_init();         // Initialize ADC
	 
	USART_init();       // Initialize USART
	
	uint16_t lightValue = 0;
	uint8_t ledBrightness = 0;
	
	while (1) {
		lightValue = 0;
		// Averaging over 10 readings
		for (int i = 0; i < 10; i++) {
			lightValue += ADC_read(); // Read light level from phototransistor
		}
		lightValue /= 10;
		
		// Print the light value to the serial terminal (Putty)
		snprintf(buffer, sizeof(buffer), "Light: %u\r\n", lightValue);
		USART_send_string(buffer);  // Send the formatted string
		
	 
		
		_delay_ms(100); // Small delay for stability
	}
}