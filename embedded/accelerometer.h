#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <stdio.h>

// Definitions
#define F_CPU 8000000UL
#define BAUD 9600
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)

// Function declarations
void ADC_init(void);
uint16_t ADC_read(uint8_t channel);
float convert_to_g(uint16_t adc_value);
void USART_init(void);
void USART_send_char(char c);
void USART_send_string(const char* str);
void float_to_string(float value, char* buffer);

#endif
