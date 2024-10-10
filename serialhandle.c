#include <avr/eeprom.h>  // Include EEPROM library
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#define F_CPU 8000000UL    // Assume a clock of 16 MHz
#define BAUD 9600
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)

struct Parameters {
    uint8_t alarm1;
    uint8_t alarm2;
    char displayMessage1;
    char displayMessage2;
    uint8_t notificationLED1;
    uint8_t notificationLED2;
    uint8_t buzzer1;
    uint8_t buzzer2;
     
    uint8_t hour;
    uint8_t minute;

    uint8_t date;
    uint8_t month;
    uint16_t year;
    uint16_t weather; //date? temp, humidity, forecast
};


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

// Function to receive data via UART
char uart_receive(void) {
	while (!(UCSR0A & (1<<RXC0)));  // Wait for data to be received
	return UDR0;                    // Get and return received data from buffer
}


void USART_send_string(const char* str) {
	while (*str) {
		USART_send_char(*str++);
	}
}

 

 

//	}


int main(void) {
	 
	USART_init();       // Initialize USART
 
	while (1) {
		 
		
		_delay_ms(300); // Small delay for stability
	}
}