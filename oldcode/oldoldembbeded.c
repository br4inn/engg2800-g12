#define F_CPU 8000000UL //what got added to make it work
#include <avr/io.h>
#include <util/delay.h>
#include <ctype.h>
#include <avr/eeprom.h>
#include <string.h>
#include <stdio.h>

#define NO_OF_STRING_PARAMETERS 17
#define NO_OF_FLOAT_PARAMETERS 14
#define NO_OF_INT_PARAMETERS 29


struct ParameterAddresses { //EEPROM addresses range from 0 to 999
	#define MESSAGE1 0
	#define MESSAGE2 (MESSAGE1 + 257) // allows room for max no. of chars allowed for alarm messages and a "\0"
	#define CELSIUS_FARENHEIT (MESSAGE2 + 257)
	#define WEATHER1_DATE (CELSIUS_FARENHEIT + 1)
	#define WEATHER2_DATE (WEATHER1_DATE + 10)
	#define WEATHER3_DATE (WEATHER2_DATE + 10)
	#define WEATHER4_DATE (WEATHER3_DATE + 10)
	#define WEATHER5_DATE (WEATHER4_DATE + 10)
	#define WEATHER6_DATE (WEATHER5_DATE + 10)
	#define WEATHER7_DATE (WEATHER6_DATE + 10)
	#define WEATHER1_FORECAST (WEATHER7_DATE + 10) // 10 seems like enough space in memory
	#define WEATHER2_FORECAST (WEATHER1_FORECAST + 10)
	#define WEATHER3_FORECAST (WEATHER2_FORECAST + 10)
	#define WEATHER4_FORECAST (WEATHER3_FORECAST + 10)
	#define WEATHER5_FORECAST (WEATHER4_FORECAST + 10)
	#define WEATHER6_FORECAST (WEATHER5_FORECAST + 10)
	#define WEATHER7_FORECAST (WEATHER6_FORECAST + 10)

	// Float parameters
	#define WEATHER1_TEMPERATURE (WEATHER7_FORECAST + 10)
	#define WEATHER1_HUMIDITY (WEATHER1_TEMPERATURE + sizeof(float))
	#define WEATHER2_TEMPERATURE (WEATHER1_HUMIDITY + sizeof(float))
	#define WEATHER2_HUMIDITY (WEATHER2_TEMPERATURE + sizeof(float))
	#define WEATHER3_TEMPERATURE (WEATHER2_HUMIDITY + sizeof(float))
	#define WEATHER3_HUMIDITY (WEATHER3_TEMPERATURE + sizeof(float))
	#define WEATHER4_TEMPERATURE (WEATHER3_HUMIDITY + sizeof(float))
	#define WEATHER4_HUMIDITY (WEATHER4_TEMPERATURE + sizeof(float))
	#define WEATHER5_TEMPERATURE (WEATHER4_HUMIDITY + sizeof(float))
	#define WEATHER5_HUMIDITY (WEATHER5_TEMPERATURE + sizeof(float))
	#define WEATHER6_TEMPERATURE (WEATHER5_HUMIDITY + sizeof(float))
	#define WEATHER6_HUMIDITY (WEATHER6_TEMPERATURE + sizeof(float))
	#define WEATHER7_TEMPERATURE (WEATHER6_HUMIDITY + sizeof(float))
	#define WEATHER7_HUMIDITY (WEATHER7_TEMPERATURE + sizeof(float))

	// Integer parameters
	#define ALARM1_SET (WEATHER7_HUMIDITY + sizeof(float))
	#define DISPLAY_MESSAGE1 (ALARM1_SET + sizeof(int))
	#define NOTIFICATION_LED1 (DISPLAY_MESSAGE1 + sizeof(int))
	#define BUZZER1 (NOTIFICATION_LED1 + sizeof(int))
	#define HOUR1_ENTRY (BUZZER1 + sizeof(int))
	#define MINUTE1_ENTRY (HOUR1_ENTRY + sizeof(int))
	#define AM_PM1_ENTRY (MINUTE1_ENTRY + sizeof(int))
	#define DATE1_ENTRY (AM_PM1_ENTRY + sizeof(int))
	#define MONTH1_ENTRY (DATE1_ENTRY + sizeof(int))
	#define YEAR1_ENTRY (MONTH1_ENTRY + sizeof(int))
	#define SET_AUTO (YEAR1_ENTRY + sizeof(int))
	#define ALARM2_SET (SET_AUTO + sizeof(int))
	#define DISPLAY_MESSAGE2 (ALARM2_SET + sizeof(int))
	#define NOTIFICATION_LED2 (DISPLAY_MESSAGE2 + sizeof(int))
	#define BUZZER2 (NOTIFICATION_LED2 + sizeof(int))
	#define HOUR2_ENTRY (BUZZER2 + sizeof(int))
	#define MINUTE2_ENTRY (HOUR2_ENTRY + sizeof(int))
	#define AM_PM2_ENTRY (MINUTE2_ENTRY + sizeof(int))
	#define DATE2_ENTRY (AM_PM2_ENTRY + sizeof(int))
	#define MONTH2_ENTRY (DATE2_ENTRY + sizeof(int))
	#define YEAR2_ENTRY (MONTH2_ENTRY + sizeof(int))
	#define VALUE_USB_OPTIC (YEAR2_ENTRY + sizeof(int))
	#define VALUE12_24 (VALUE_USB_OPTIC + sizeof(int))
	#define AM_PM (VALUE12_24 + sizeof(int))
	#define HOUR (AM_PM + sizeof(int))
	#define MINUTE (HOUR + sizeof(int))
	#define DAY (MINUTE + sizeof(int))
	#define MONTH (DAY + sizeof(int))
	#define YEAR (MONTH + sizeof(int))


};

//need an array with all the constants in the same order so that we can access from the order as we loop
//through the received parameter arrays
const int parameter_order[] = {
	// String parameters:
	MESSAGE1,
	MESSAGE2,
	CELSIUS_FARENHEIT,
	WEATHER1_DATE,
	WEATHER2_DATE,
	WEATHER3_DATE,
	WEATHER4_DATE,
	WEATHER5_DATE,
	WEATHER6_DATE,
	WEATHER7_DATE,
	WEATHER1_FORECAST,
	WEATHER2_FORECAST,
	WEATHER3_FORECAST,
	WEATHER4_FORECAST,
	WEATHER5_FORECAST,
	WEATHER6_FORECAST,
	WEATHER7_FORECAST,

	// Float parameters:
	WEATHER1_TEMPERATURE,
	WEATHER1_HUMIDITY,
	WEATHER2_TEMPERATURE,
	WEATHER2_HUMIDITY,
	WEATHER3_TEMPERATURE,
	WEATHER3_HUMIDITY,
	WEATHER4_TEMPERATURE,
	WEATHER4_HUMIDITY,
	WEATHER5_TEMPERATURE,
	WEATHER5_HUMIDITY,
	WEATHER6_TEMPERATURE,
	WEATHER6_HUMIDITY,
	WEATHER7_TEMPERATURE,
	WEATHER7_HUMIDITY,

	// Integer parameters:
	ALARM1_SET,
	DISPLAY_MESSAGE1,
	NOTIFICATION_LED1,
	BUZZER1,
	HOUR1_ENTRY,
	MINUTE1_ENTRY,
	AM_PM1_ENTRY,
	DATE1_ENTRY,
	MONTH1_ENTRY,
	YEAR1_ENTRY,
	SET_AUTO,
	ALARM2_SET,
	DISPLAY_MESSAGE2,
	NOTIFICATION_LED2,
	BUZZER2,
	HOUR2_ENTRY,
	MINUTE2_ENTRY,
	AM_PM2_ENTRY,
	DATE2_ENTRY,
	MONTH2_ENTRY,
	YEAR2_ENTRY,
	VALUE_USB_OPTIC,
	VALUE12_24,
	AM_PM,
	HOUR,
	MINUTE,
	DAY,
	MONTH,
	YEAR
};

//receive the JSON data
//decode the JSON data back into a 2D array

//currently uses fake data:
struct Parameters {
	// Declare the string parameters array
	char string_parameters[17][15]; //cannot be 15, need to use pointers with dynamically assigned memory
	
	// Declare the float parameters array
	float float_parameters[14];
	
	// Declare the integer parameters array
	int int_parameters[29];
	
};

// Declare functions
void uart_init(unsigned int baud);
void uart_send(char data);
char uart_receive(void);
void EEPROM_write_string(unsigned int address, char* data);
void EEPROM_write_int(unsigned int address, int data);
void write_parameters_to_EEPROM(struct Parameters* parameters);
void read_all_EEPROM();


int main(void) {
	
	// Initialize the Parameters struct instance
	struct Parameters parameters = {
		// Initialize the string array
		{
			"Alarm 1 message",
			"Alarm 2 message",
			"C",
			"2024-09-20",
			"2024-09-21",
			"2024-09-22",
			"2024-09-23",
			"2024-09-24",
			"2024-09-25",
			"2024-09-26",
			"Sunny",
			"Cloudy",
			"Rainy",
			"Windy",
			"Stormy",
			"Foggy",
			"Clear"
		},
		
		// Initialize the float array
		{
			25.123,
			60.456,
			24.789,
			65.321,
			22.654,
			70.987,
			20.111,
			75.222,
			18.333,
			80.444,
			17.555,
			85.666,
			16.777,
			90.888
		},
		
		// Initialize the integer array
		{
			1,
			0,
			1,
			1,
			8,
			30,
			0,
			20,
			9,
			2024,
			1,
			0,
			1,
			0,
			0,
			9,
			0,
			1,
			21,
			9,
			2023,
			0,
			1,
			0,
			10,
			15,
			20,
			9,
			2023,
		}
	};
	
	uart_init(9600);  // Initialize UART with 9600 baud rate
	
	DDRB = 0xFF;
	PORTB = 0xFF;

	//will need to move into the while loop once using real received data (if received, write_parameters_to_EEPROM())
	write_parameters_to_EEPROM(&parameters); //Parsing the pointer is more efficient
	printf("Parameters have been written to EEPROM\n");
	read_all_EEPROM();
	
	while (1) {
		char received = uart_receive();  // Receive data via UART
		uart_send(received);  // Send back the received data
	}
	
	return 0;
}

// Function to initialize UART communication
void uart_init(unsigned int baud) {
	unsigned int ubrr = F_CPU/16/baud - 1;
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);  // Enable receiver and transmitter
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); // Set frame format: 8 data bits, 1 stop bit
}

// Function to send data via UART
void uart_send(char data) {
	while (!(UCSR0A & (1<<UDRE0)));  // Wait for empty transmit bufferF
	1 << PORTD2;
	UDR0 = data;                     // Send data
}

// Function to receive data via UART
char uart_receive(void) {
	while (!(UCSR0A & (1<<RXC0)));  // Wait for data to be received
	return UDR0;                    // Get and return received data from buffer
}


// Function to write a string to EEPROM
void EEPROM_write_string(unsigned int address, char* data) {
	unsigned char * char_address = (unsigned char *)&address;
	for (int i = 0; i < strlen(data); i++) {
		eeprom_write_byte(char_address + i, *data + i);
	}
	
	eeprom_write_byte(char_address + strlen(data), '\0');  // Null-terminate the string
}

// Function to write an integer to EEPROM
void EEPROM_write_int(unsigned int address, int data) {
	unsigned char * char_address = (unsigned char *)&address;
	unsigned char* intPtr = (unsigned char*)&data;
	for (int i = 0; i < sizeof(int); i++) {
		eeprom_write_byte(char_address + i, intPtr[i]);
	}
}

void write_parameters_to_EEPROM(struct Parameters* parameters) {
	int eeprom_address;
	
	//process the string parameters
	for (int j=0; j < NO_OF_STRING_PARAMETERS; j++) {
		eeprom_address = parameter_order[j];
		// Use -> as parameters (structure) is now parsed into the function as a pointer
		EEPROM_write_string(eeprom_address, parameters->string_parameters[j]); 


	}

	//process the float parameters
		for (int j = 0; j < sizeof(parameters->float_parameters) / sizeof(float); j++) {
			eeprom_address = parameter_order[j];
			eeprom_write_float((float *)eeprom_address, parameters->float_parameters[j]); //in built function of <avr/eeprom.h>
		}
		
	//process the integer parameters
	for (int j = 0; j < sizeof(parameters->int_parameters) / sizeof(int); j++) {
		eeprom_address = parameter_order[j];
		EEPROM_write_int(eeprom_address, parameters->int_parameters[j]);
	}
}

/*prints all bytes of EEPROM to terminal as unsigned chars (unprocessed)*/
void read_all_EEPROM() {
	for (int i = 0; i < 999; i++) {
		const uint8_t * address = (unsigned char*)i;
		uint8_t value = eeprom_read_byte(address);
		printf("%u\n", value);

	}
}


