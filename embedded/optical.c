#include <avr/eeprom.h>  // Include EEPROM library
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/interrupt.h>  // Required for ISR
#define F_CPU 8000000UL    // Assume a clock of 8 MHz
#define BAUD 9600
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)

#define MAX_DATA_SIZE 32 // 27 sent
#define PHOTODIODE PC0       // Photodiode connected to PC0
#define BUTTON PD3


volatile uint32_t elapsed_time_ms = 0; // Milliseconds elapsed
// Global variables
uint8_t data[MAX_DATA_SIZE]; // Array to store bits
uint16_t bit_index;          // Index for storing bits in the array
//extern char dateandtime[17];          // For example "12:30 PM 29/09/2024"
uint8_t transfer_complete = 0;

uint32_t binary_to_int(const char* bin_str);
char decode_colour(uint16_t adc_value);
void USART_init();
void USART_send_char(char c);
void USART_send_string(const char* str);
void ADC_init();
uint16_t ADC_read();
void init_optic_button();

void handle_colour(uint8_t bit);
void read_from_eeprom();
void store_data(uint8_t bit);
void process_binary_data(char *binary_data);

// Timer/Counter interrupt setup
void timer_init() {
	// Set Timer/Counter0 to CTC mode
	TCCR0A = (1 << WGM01); // Set to CTC mode
	TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler of 64
	OCR0A = 124; // 300 ms at 8 MHz with 64 prescaler (F_CPU / 64 / 1000) - 1 = 124
	TIMSK0 |= (1 << OCIE0A); // Enable Timer/Counter0 Output Compare Match A Interrupt
}

// Timer/Counter0 Compare Match A ISR
ISR(TIMER0_COMPA_vect) {
	elapsed_time_ms++; // Increment elapsed time each 1 ms
}

// Custom delay function
void custom_delay_ms(uint32_t ms) {
	elapsed_time_ms = 0; // Reset elapsed time
	while (elapsed_time_ms < ms) {
		// Wait until the required time has passed
	}
}

int main(void) {
	// Initialize UART, ADC, and button
	USART_init();
	ADC_init();
	timer_init();
	sei();
	uint16_t brightness = 0;
	char buffer[32];
	char bufferButton[32];
	uint16_t transfer_enabled = 0;

	//init_optic_button();

	
	while (1) {
		// Check if button is pressed (active-low)
		//	if (!(PIND & (1 << BUTTON))) {
		//		transfer_enabled = 1;  // Enable transfer when button is pressed
		//	snprintf(bufferButton, sizeof(bufferButton), "Button pressed :)");
		//	USART_send_string(bufferButton);
		//} else {
		//transfer_enabled = 0;  // Disable transfer when button is released
		//}

		//	if (transfer_enabled) {

		// Average brightness level
		brightness = 0;
		for (int i = 0; i < 10; i++) {
			brightness += ADC_read();
		}
		brightness /= 10;

		char bit = decode_colour(brightness);
		store_data(bit);

		//if (transfer_complete) {
		//read_from_eeprom();  // Read and decode stored data

		
		//	transfer_complete = 0;  // Reset transfer flag
		//	}

		snprintf(buffer, sizeof(buffer), "Bit interpreted: %c\r\n", bit);

		USART_send_string(buffer);

		custom_delay_ms(300); // so we can only do max 30 ms delay with this- we should  write our own delay function that checks the elapsed time
		// keep in mind serial will mess with this
	}
	
	//	}
}

void USART_init() {
	UBRR0H = (BAUD_PRESCALE >> 8);
	UBRR0L = BAUD_PRESCALE;
	UCSR0B = (1 << TXEN0);
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
}

void USART_send_char(char c) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

void USART_send_string(const char* str) {
	while (*str) {
		USART_send_char(*str++);
	}
}

void ADC_init() {
	ADMUX = (1 << REFS0);  // AVcc as reference, ADC0 (PC0) as input
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); // Enable ADC, prescaler of 64
}

uint16_t ADC_read() {
	ADCSRA |= (1 << ADSC); // Start conversion
	while (ADCSRA & (1 << ADSC));
	return ADC;
}

void init_optic_button() {
	DDRD &= ~(1 << BUTTON);  // button pd3
	PORTD |= (1 << BUTTON);  // Enable internal pull-up resistor
}

char bit_buffer[34] = {0};  // Reserve space for 33 bits + '\0'


char decode_colour(uint16_t adc_value) {
	uint8_t bit_count = 0;  // Keep track of how many bits are stored
	// Check if bit_count is within bounds
	if (bit_count < 33) {
		if (adc_value >= 0 && adc_value <= 15) {
			bit_buffer[bit_count] = '0';  // Black
			bit_count++;
			} else if (adc_value >= 50 && adc_value <= 120) {
			bit_buffer[bit_count] = '1';  // White
			bit_count++;
			} else if (adc_value >= 20 && adc_value <= 43) {
			bit_buffer[bit_count] = 'S';  // Red (Start/Stop bit)
			bit_count++;
			} else {
			return 'U';  // Unknown color
		}
		} else {
		return 'F';
	}

	return bit_buffer[bit_count - 1];  // Return the last stored bit
}


uint32_t binary_to_int(const char* bin_str) {
	uint32_t value = 0;
	while (*bin_str) {
		value = (value << 1) | (*bin_str++ - '0');
	}
	return value;
}

void store_data(uint8_t bit) {
	static uint8_t storing_data = 0;
	static uint16_t eeprom_address = 0;
	static char binary_data[MAX_DATA_SIZE + 1] = {0};  // Ensure it's initialized to 0
	static uint8_t bit_count = 0;  // Counter to keep track of how many bits are added
	char debug_buffer[128];
	char buf[128];

	if (bit == 'S') {  // Start or stop bit detected
		if (!storing_data) {
			// Start storing data
			storing_data = 1;
			eeprom_address = 0;
			bit_count = 0;  // Reset bit count when storage starts 
			USART_send_string("Start bit detected. Data storage started.\r\n");
			} else {
			// Stop storing data and process
			storing_data = 0;
			binary_data[eeprom_address] = '\0';  // Null-terminate the binary string
			
			snprintf(debug_buffer, sizeof(debug_buffer), "bin before storing: %s\r\n", binary_data);
			USART_send_string(debug_buffer);

			snprintf(buf, sizeof(buf), "Total bits stored: %d\r\n", bit_count);
			USART_send_string(buf);
			
			// Process the data after stopping
			process_binary_data(binary_data);
			USART_send_string("Stop bit detected. Data storage complete.\r\n");
		}
	}
	else if (storing_data && (bit == '0' || bit == '1')) {
		// Store the bit if we're actively storing data and address is within bounds
		if (eeprom_address < MAX_DATA_SIZE) {
			binary_data[eeprom_address++] = bit;  // Store the bit
			bit_count++;   
		}
	}
}

// Process the binary data after storage
void process_binary_data(char *binary_data) {
	char hour_string[6] = {0};   // 5 bit hour + null
	char min_string[7] = {0};    // 6 bit minute
	char day_string[6] = {0};    // 5 bit day
	char month_string[5] = {0};  // 4 bit month
	char year_string[13] = {0};  // 12 bit year

	// Copy relevant parts of binary data
	strncpy(hour_string, binary_data, 5);
	strncpy(min_string, binary_data + 5, 6);
	strncpy(day_string, binary_data + 11, 5);
	strncpy(month_string, binary_data + 16, 4);
	strncpy(year_string, binary_data + 20, 12);

	// Convert binary strings to integers
	int hour = binary_to_int(hour_string);
	int minute = binary_to_int(min_string);
	int day = binary_to_int(day_string);
	int month = binary_to_int(month_string);
	int year = binary_to_int(year_string);
	char dateandtime[17];
	
	snprintf(dateandtime, sizeof(dateandtime), "%02d:%02d %02d/%02d/%04d\n", hour, minute, day, month, year);

	USART_send_string(dateandtime);
}


