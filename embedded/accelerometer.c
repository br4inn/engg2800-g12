#include "accelerometer.h"


void ADC_init() {
	ADMUX = (1 << REFS0);
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
}

// Read a value from the selected ADC channel (0-7), 0 for diode, 1 for y axis, 2 for x axis, 3 for z axis
uint16_t ADC_read(uint8_t channel) {
	// Select channel
	ADMUX = (ADMUX & 0xF8) | (channel & 0x07);
	ADCSRA |= (1 << ADSC);
	
	while (ADCSRA & (1 << ADSC));
	
	return ADCW;
}

// Convert ADC value to g-force
float convert_to_g(uint16_t adc_value) {
	// 3.3V reference, 10-bit ADC
	float voltage = (adc_value * 3.3) / 1024.0;
	
	// 1.65V is zero-g, 330mV/g sensitivity at 3.3V
	float gForce = (voltage - 1.65) / 0.33;
	
	return gForce;
}

void read_accelerometer(float *x, float *y, float *z) {
	uint16_t xADC = ADC_read(2);  // XOUT on PC2 (ADC2)
	uint16_t yADC = ADC_read(1);  // YOUT on PC1 (ADC1)
	uint16_t zADC = ADC_read(3);  // ZOUT on PC3 (ADC3)
	
	*x = convert_to_g(xADC);
	*y = convert_to_g(yADC);
	*z = convert_to_g(zADC);
}

float calc_pitch(float x, float y, float z) {
	return atan2(x, sqrt(y * y + z * z)) * 180 / M_PI;
}

float calc_roll(float x, float y, float z) {
	return atan2(y, sqrt(x * x + z * z)) * 180 / M_PI;
}

// To be completed once display is finished...
void check_orientation(float pitch, float roll) {
	if (pitch > 40.0) {
		// Rotate the screen 180 degrees and remap buttons
	 
		} else {
		// Keep the original orientation
	 
	}
}

void USART_init() {
	// Set baud rate
	UBRR0H = (BAUD_PRESCALE >> 8);
	UBRR0L = BAUD_PRESCALE;
	
	// Enable transmitter
	UCSR0B = (1 << TXEN0);
	
	// Set frame format: 8 data bits, 1 stop bit
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
}

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

// Function to convert float to string
void float_to_string(float value, char* buffer) {
	int intPart = (int)value; // Get integer part
	int decimalPart = (int)((value - intPart) * 100); // two decimal places
	sprintf(buffer, "%d.%02d", intPart, abs(decimalPart)); // Format as string
}

int main(void) {
	float x, y, z;
	float pitch, roll;
	char buffer[50];
	
	USART_init();  
	ADC_init();     
	
	while (1) {
 
		read_accelerometer(&x, &y, &z);
		 
		pitch = calc_pitch(x, y, z);
		roll = calc_roll(x, y, z);
		 
		float_to_string(x, buffer);
		USART_send_string("X: ");
		USART_send_string(buffer);
		USART_send_string(" g, ");
		
		float_to_string(y, buffer);
		USART_send_string("Y: ");
		USART_send_string(buffer);
		USART_send_string(" g, ");
		
		float_to_string(z, buffer);
		USART_send_string("Z: ");
		USART_send_string(buffer);
		USART_send_string(" g\n");
		
 
		float_to_string(pitch, buffer);
		USART_send_string("Pitch: ");
		USART_send_string(buffer);
		USART_send_string(", ");
		
		float_to_string(roll, buffer);
		USART_send_string("Roll: ");
		USART_send_string(buffer);
		USART_send_string("\n");
		 
		check_orientation(pitch, roll);
		
		_delay_ms(500);  
	}
}
