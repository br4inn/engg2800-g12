#include <avr/eeprom.h>  // Include EEPROM library
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#define F_CPU 8000000UL    // Assume a clock of 8 MHz
#define BAUD 9600
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)

#define MAX_DATA_SIZE 27  // 27 sent 
#define PHOTODIODE PC0       // Photodiode connected to PC0

// Global variables
uint8_t data[MAX_DATA_SIZE]; // Array to store bits
uint16_t bit_index;          // Index for storing bits in the array
char hour, minute, day, month;
uint16_t year;               // Year with an offset of 2000
uint8_t transfer_complete = 0;

char decode_colour(uint16_t adc_value);
void USART_init();
void USART_send_char(char c);
void USART_send_string(const char* str);
void ADC_init();
uint16_t ADC_read();

void handle_colour(uint8_t bit); 
void read_from_eeprom();

int main(void) {
    // Initialize UART and ADC
    USART_init();
    ADC_init();
    
    uint16_t lightVal = 0;
  
    char buffer[32];
    char buffer2[256];

    while (1) {
        lightVal = 0;
        for (int i = 0; i < 10; i++) {
            lightVal += ADC_read();
        }
        lightVal /= 10;

        char bit = decode_colour(lightVal);
        handle_colour(bit);

        if (transfer_complete) {
            read_from_eeprom();  // Read and decode stored data
   
	 
            snprintf(buffer2, sizeof(buffer2), "Treading from stored:ime: %s:%s\nDate: %s/%s/%s\n", hour, minute, day, month, year);
            USART_send_string(buffer2);

            transfer_complete = 0;  // Reset transfer flag
        }

        snprintf(buffer, sizeof(buffer), "Bit interpreted: %u\r\n", bit);
        USART_send_string(buffer);

        _delay_ms(300);
    }
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

char decode_colour(uint16_t adc_value) {
    if (adc_value >= 0 && adc_value <= 10) {
        return '0';  // Black  
    } else if (adc_value >= 50 && adc_value <= 120) {
        return '1';  // White  
    } else if (adc_value >= 20 && adc_value <= 30) {
        return 'S';  //  red-start/Stop bit
    } else {
        return 'U';  // Unknown  
    }
}

// Handle color and store data in EEPROM using block write
void handle_colour(uint8_t bit) {
    static uint8_t storing_data = 0;
    static uint16_t eeprom_address = 0;
    static uint8_t eeprom_data[MAX_DATA_SIZE + 1 ]; //+1 for NULL?
    char debug_buffer[128];
    if (bit == 'S') {  // Red indicates start or stop bit
        if (!storing_data) {
            storing_data = 1;  // Start storing
            eeprom_address = 0;
        } else {
            eeprom_data[eeprom_address] = '\0';  // Null-terminate 
            
            //  convert binary string to integer
            long dateTime = strtol(eeprom_data, NULL, 2);  
            
	        snprintf(debug_buffer, sizeof(debug_buffer), "binto str before stroing: %s \r\n", dateTime);
	        USART_send_string(debug_buffer);
             
            eeprom_update_block((const void*)dateTime, (void*)0x00, sizeof(eeprom_data));  
            
            storing_data = 0;  // Stop  
        }
    } else if (colour == '0' || colour == '1') {   
        if (storing_data && eeprom_address < MAX_DATA_SIZE) {
            eeprom_data[eeprom_address++] = bit;  
        }
    }
}



 

// Read and decode data from EEPROM
void read_from_eeprom() {

    char debug_buffer[128];
     char stored_data[MAX_DATA_SIZE + 1];   

    // Read the stored string from EEPROM
    eeprom_read_block((void*)stored_data, (const void*)0x00, MAX_DATA_SIZE);
    stored_data[MAX_DATA_SIZE] = '\0';  //   null terminate

 
    char hour[3] = { stored_data[0], stored_data[1], '\0' };
    char minute[3] = { stored_data[2], stored_data[3], '\0' };
    char day[3] = { stored_data[4], stored_data[5], '\0' };
    char month[3] = { stored_data[6], stored_data[7], '\0' };
    char year[5] = "2000";   



    // Concatenate the last two digits of the year to "2000"
    year[2] = stored_data[8];
    year[3] = stored_data[9];
    year[4] = '\0';  // Null-terminate the year string
    
    transfer_complete = 1;
}
