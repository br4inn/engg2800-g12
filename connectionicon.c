#define F_CPU 8000000UL //what got added to make it work
#include <avr/io.h>
#include <util/delay.h>
#include <ctype.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>  // Required for ISR
#include <string.h>  // For strtok(), strncpy(), and other string functions
#include <stdlib.h>  // For atoi(), atof(), and other standard functions
#include <stdio.h>  // For snprintf()

#define MESSAGE_SIZE 256
#define WEATHER_DATE_SIZE 10
#define FORECAST_SIZE 10


unsigned long lastCheckTime = 0;
const unsigned long checkInterval = 1000; // How often connection is checked


volatile uint32_t elapsed_time_ms = 0; // Milliseconds elapsed
#define BUFFER_SIZE 1024

typedef struct {
	char message1[MESSAGE_SIZE];
	char message2[MESSAGE_SIZE];
	uint8_t celsius_fahrenheit;
	char weather1Date[WEATHER_DATE_SIZE];
	char weather2Date[WEATHER_DATE_SIZE];
	char weather3Date[WEATHER_DATE_SIZE];
	char weather4Date[WEATHER_DATE_SIZE];
	char weather5Date[WEATHER_DATE_SIZE];
	char weather6Date[WEATHER_DATE_SIZE];
	char weather7Date[WEATHER_DATE_SIZE];
	char weather1Forecast[FORECAST_SIZE];
	char weather2Forecast[FORECAST_SIZE];
	char weather3Forecast[FORECAST_SIZE];
	char weather4Forecast[FORECAST_SIZE];
	char weather5Forecast[FORECAST_SIZE];
	char weather6Forecast[FORECAST_SIZE];
	char weather7Forecast[FORECAST_SIZE];
	float weather1Temp;
	float weather1Humidity;
	float weather2Temp;
	float weather2Humidity;
	float weather3Temp;
	float weather3Humidity;
	float weather4Temp;
	float weather4Humidity;
	float weather5Temp;
	float weather5Humidity;
	float weather6Temp;
	float weather6Humidity;
	float weather7Temp;
	float weather7Humidity;
	uint8_t alarm1Set;
	uint16_t displayMessage1;
	uint16_t notificationLED1;
	uint16_t buzzer1;
	uint16_t hour1;
	uint16_t minute1;
	uint16_t ampm1;
	uint16_t date1;
	uint16_t month1;
	uint16_t year1;
	uint16_t setAuto;
	uint8_t alarm2Set;
	uint16_t displayMessage2;
	uint16_t notificationLED2;
	uint16_t buzzer2;
	uint16_t hour2;
	uint16_t minute2;
	uint16_t ampm2;
	uint16_t date2;
	uint16_t month2;
	uint16_t year2;
	uint16_t valueUSBOptic;
	uint16_t value12_24;
	uint16_t ampm;
	uint16_t hour;
	uint16_t minute;
	uint16_t day;
	uint16_t month;
	uint16_t year;
} Parameters;

Parameters received_params;

// EEPROM memory allocations for all parameters
char EEMEM message1EEPROM[MESSAGE_SIZE];
char EEMEM message2EEPROM[MESSAGE_SIZE];
uint8_t EEMEM celsiusFahrenheitEEPROM;
char EEMEM weather1DateEEPROM[WEATHER_DATE_SIZE];
char EEMEM weather1ForecastEEPROM[FORECAST_SIZE];
char EEMEM weather2DateEEPROM[WEATHER_DATE_SIZE];
char EEMEM weather2ForecastEEPROM[FORECAST_SIZE];
char EEMEM weather3DateEEPROM[WEATHER_DATE_SIZE];
char EEMEM weather3ForecastEEPROM[FORECAST_SIZE];
char EEMEM weather4DateEEPROM[WEATHER_DATE_SIZE];
char EEMEM weather4ForecastEEPROM[FORECAST_SIZE];
char EEMEM weather5DateEEPROM[WEATHER_DATE_SIZE];
char EEMEM weather5ForecastEEPROM[FORECAST_SIZE];
char EEMEM weather6DateEEPROM[WEATHER_DATE_SIZE];
char EEMEM weather6ForecastEEPROM[FORECAST_SIZE];
char EEMEM weather7DateEEPROM[WEATHER_DATE_SIZE];
char EEMEM weather7ForecastEEPROM[FORECAST_SIZE];
float EEMEM weather1TempEEPROM;
float EEMEM weather1HumidityEEPROM;
float EEMEM weather2TempEEPROM;
float EEMEM weather2HumidityEEPROM;
float EEMEM weather3TempEEPROM;
float EEMEM weather3HumidityEEPROM;
float EEMEM weather4TempEEPROM;
float EEMEM weather4HumidityEEPROM;
float EEMEM weather5TempEEPROM;
float EEMEM weather5HumidityEEPROM;
float EEMEM weather6TempEEPROM;
float EEMEM weather6HumidityEEPROM;
float EEMEM weather7TempEEPROM;
float EEMEM weather7HumidityEEPROM;
uint16_t EEMEM alarm1SetEEPROM;
uint16_t EEMEM displayMessage1EEPROM;
uint16_t EEMEM notificationLED1EEPROM;
uint16_t EEMEM buzzer1EEPROM;
uint16_t EEMEM hour1EEPROM;
uint16_t EEMEM minute1EEPROM;
uint16_t EEMEM ampm1EEPROM;
uint16_t EEMEM date1EEPROM;
uint16_t EEMEM month1EEPROM;
uint16_t EEMEM year1EEPROM;
uint16_t EEMEM setAutoEEPROM;
uint16_t EEMEM alarm2SetEEPROM;
uint16_t EEMEM displayMessage2EEPROM;
uint16_t EEMEM notificationLED2EEPROM;
uint16_t EEMEM buzzer2EEPROM;
uint16_t EEMEM hour2EEPROM;
uint16_t EEMEM minute2EEPROM;
uint16_t EEMEM ampm2EEPROM;
uint16_t EEMEM date2EEPROM;
uint16_t EEMEM month2EEPROM;
uint16_t EEMEM year2EEPROM;
uint16_t EEMEM valueUSBOpticEEPROM;
uint16_t EEMEM value12_24EEPROM;
uint16_t EEMEM ampmEEPROM;
uint16_t EEMEM hourEEPROM;
uint16_t EEMEM minuteEEPROM;
uint16_t EEMEM dayEEPROM;
uint16_t EEMEM monthEEPROM;
uint16_t EEMEM yearEEPROM;

void load_data_from_EEPROM();
void send_eeprom_data_to_gui();
void send_data_to_gui();
int checkConnection();

// Function to initialize UART communication
void uart_init(unsigned int baud) {
	unsigned int ubrr = F_CPU/16/baud - 1;
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);  // Enable receiver and transmitter
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); // Set frame format: 8 data bits, 1 stop bit
}
/*
// Function to send data via UART
void uart_send(char data) {
	while (!(UCSR0A & (1<<UDRE0)));  // Wait for empty transmit buffer
	1 << PORTD2;
	UDR0 = data;                     // Send data
}
*/
void uart_send_char(char data) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

// UART send string function (
void uart_send_string(const char *str) {
	while (*str) {
		uart_send_char(*str++);
	}
}


// Function to receive data via UART with approximate timeout
char uart_receive(void) {
    const unsigned long timeoutCount = 500; // Timout count (5 secs)
    unsigned long counter = 0;

    while (!(UCSR0A & (1<<RXC0))) { // Wait for data to be received
        if (counter++ >= timeoutCount) {
            return '\0'; // Return null character if timeout occurs
        }
        _delay_ms(10); // Small delay to control the loop speed
    }

    return UDR0; // Get and return received data from buffer
}

/*
//Original
char uart_receive(void) {
	while (!(UCSR0A & (1<<RXC0)));  // Wait for data to be received
	return UDR0;                    // Get and return received data from buffer
}
*/


void parse_received_data(char *data) { //need to make sure its parameter data and not connection icon data
	char *token;

	// Parse message1
	token = strtok(data, ",");
	if (token != NULL) {
		strncpy(received_params.message1, token, MESSAGE_SIZE - 1);
		received_params.message1[MESSAGE_SIZE - 1] = '\0';  // Ensure null-termination
	}

	// Parse message2
	token = strtok(NULL, ",");
	if (token != NULL) {
		strncpy(received_params.message2, token, MESSAGE_SIZE - 1);
		received_params.message2[MESSAGE_SIZE - 1] = '\0';  // Ensure null-termination
	}

	// Parse celsius_fahrenheit
	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.celsius_fahrenheit = (uint8_t)atoi(token);
	}

	// Parse weather dates
	token = strtok(NULL, ",");
	if (token != NULL) {
		strncpy(received_params.weather1Date, token, WEATHER_DATE_SIZE - 1);
		received_params.weather1Date[WEATHER_DATE_SIZE - 1] = '\0';
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		strncpy(received_params.weather2Date, token, WEATHER_DATE_SIZE - 1);
		received_params.weather2Date[WEATHER_DATE_SIZE - 1] = '\0';
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		strncpy(received_params.weather3Date, token, WEATHER_DATE_SIZE - 1);
		received_params.weather3Date[WEATHER_DATE_SIZE - 1] = '\0';
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		strncpy(received_params.weather4Date, token, WEATHER_DATE_SIZE - 1);
		received_params.weather4Date[WEATHER_DATE_SIZE - 1] = '\0';
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		strncpy(received_params.weather5Date, token, WEATHER_DATE_SIZE - 1);
		received_params.weather5Date[WEATHER_DATE_SIZE - 1] = '\0';
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		strncpy(received_params.weather6Date, token, WEATHER_DATE_SIZE - 1);
		received_params.weather6Date[WEATHER_DATE_SIZE - 1] = '\0';
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		strncpy(received_params.weather7Date, token, WEATHER_DATE_SIZE - 1);
		received_params.weather7Date[WEATHER_DATE_SIZE - 1] = '\0';
	}

	// Parse weather forecasts
	token = strtok(NULL, ",");
	if (token != NULL) {
		strncpy(received_params.weather1Forecast, token, FORECAST_SIZE - 1);
		received_params.weather1Forecast[FORECAST_SIZE - 1] = '\0';
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		strncpy(received_params.weather2Forecast, token, FORECAST_SIZE - 1);
		received_params.weather2Forecast[FORECAST_SIZE - 1] = '\0';
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		strncpy(received_params.weather3Forecast, token, FORECAST_SIZE - 1);
		received_params.weather3Forecast[FORECAST_SIZE - 1] = '\0';
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		strncpy(received_params.weather4Forecast, token, FORECAST_SIZE - 1);
		received_params.weather4Forecast[FORECAST_SIZE - 1] = '\0';
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		strncpy(received_params.weather5Forecast, token, FORECAST_SIZE - 1);
		received_params.weather5Forecast[FORECAST_SIZE - 1] = '\0';
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		strncpy(received_params.weather6Forecast, token, FORECAST_SIZE - 1);
		received_params.weather6Forecast[FORECAST_SIZE - 1] = '\0';
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		strncpy(received_params.weather7Forecast, token, FORECAST_SIZE - 1);
		received_params.weather7Forecast[FORECAST_SIZE - 1] = '\0';
	}

	// Parse weather temperatures and humidity
	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.weather1Temp = atof(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.weather1Humidity = atof(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.weather2Temp = atof(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.weather2Humidity = atof(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.weather3Temp = atof(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.weather3Humidity = atof(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.weather4Temp = atof(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.weather4Humidity = atof(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.weather5Temp = atof(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.weather5Humidity = atof(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.weather6Temp = atof(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.weather6Humidity = atof(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.weather7Temp = atof(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.weather7Humidity = atof(token);
	}

	// Parse alarm1 parameters
	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.alarm1Set = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.displayMessage1 = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.notificationLED1 = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.buzzer1 = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.hour1 = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.minute1 = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.ampm1 = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.date1 = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.month1 = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.year1 = (uint16_t)atoi(token);
	}

	// Parse alarm2 parameters
	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.alarm2Set = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.displayMessage2 = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.notificationLED2 = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.buzzer2 = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.hour2 = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.minute2 = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.ampm2 = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.date2 = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.month2 = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.year2 = (uint16_t)atoi(token);
	}

	// Parse general parameters (valueUSBOptic, 12/24 hour format, etc.)
	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.valueUSBOptic = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.value12_24 = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.ampm = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.hour = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.minute = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.day = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.month = (uint16_t)atoi(token);
	}

	token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.year = (uint16_t)atoi(token);
	}
	send_data_to_gui();
//	load_data_from_EEPROM();
	//send_eeprom_data_to_gui();
}


// Save received data into EEPROM
void save_data_to_EEPROM() {
	// Save string parameters
	eeprom_update_block((const void*)received_params.message1, (void*)message1EEPROM, MESSAGE_SIZE);
	eeprom_update_block((const void*)received_params.message2, (void*)message2EEPROM, MESSAGE_SIZE);
	eeprom_update_byte(&celsiusFahrenheitEEPROM, received_params.celsius_fahrenheit);

	// Save weather dates and forecasts
	eeprom_update_block((const void*)received_params.weather1Date, (void*)weather1DateEEPROM, WEATHER_DATE_SIZE);
	eeprom_update_block((const void*)received_params.weather1Forecast, (void*)weather1ForecastEEPROM, FORECAST_SIZE);
	eeprom_update_block((const void*)received_params.weather2Date, (void*)weather2DateEEPROM, WEATHER_DATE_SIZE);
	eeprom_update_block((const void*)received_params.weather2Forecast, (void*)weather2ForecastEEPROM, FORECAST_SIZE);
	eeprom_update_block((const void*)received_params.weather3Date, (void*)weather3DateEEPROM, WEATHER_DATE_SIZE);
	eeprom_update_block((const void*)received_params.weather3Forecast, (void*)weather3ForecastEEPROM, FORECAST_SIZE);
	eeprom_update_block((const void*)received_params.weather4Date, (void*)weather4DateEEPROM, WEATHER_DATE_SIZE);
	eeprom_update_block((const void*)received_params.weather4Forecast, (void*)weather4ForecastEEPROM, FORECAST_SIZE);
	eeprom_update_block((const void*)received_params.weather5Date, (void*)weather5DateEEPROM, WEATHER_DATE_SIZE);
	eeprom_update_block((const void*)received_params.weather5Forecast, (void*)weather5ForecastEEPROM, FORECAST_SIZE);
	eeprom_update_block((const void*)received_params.weather6Date, (void*)weather6DateEEPROM, WEATHER_DATE_SIZE);
	eeprom_update_block((const void*)received_params.weather6Forecast, (void*)weather6ForecastEEPROM, FORECAST_SIZE);
	eeprom_update_block((const void*)received_params.weather7Date, (void*)weather7DateEEPROM, WEATHER_DATE_SIZE);
	eeprom_update_block((const void*)received_params.weather7Forecast, (void*)weather7ForecastEEPROM, FORECAST_SIZE);

	// Save float parameters (weather data)
	eeprom_update_float(&weather1TempEEPROM, received_params.weather1Temp);
	eeprom_update_float(&weather1HumidityEEPROM, received_params.weather1Humidity);
	eeprom_update_float(&weather2TempEEPROM, received_params.weather2Temp);
	eeprom_update_float(&weather2HumidityEEPROM, received_params.weather2Humidity);
	eeprom_update_float(&weather3TempEEPROM, received_params.weather3Temp);
	eeprom_update_float(&weather3HumidityEEPROM, received_params.weather3Humidity);
	eeprom_update_float(&weather4TempEEPROM, received_params.weather4Temp);
	eeprom_update_float(&weather4HumidityEEPROM, received_params.weather4Humidity);
	eeprom_update_float(&weather5TempEEPROM, received_params.weather5Temp);
	eeprom_update_float(&weather5HumidityEEPROM, received_params.weather5Humidity);
	eeprom_update_float(&weather6TempEEPROM, received_params.weather6Temp);
	eeprom_update_float(&weather6HumidityEEPROM, received_params.weather6Humidity);
	eeprom_update_float(&weather7TempEEPROM, received_params.weather7Temp);
	eeprom_update_float(&weather7HumidityEEPROM, received_params.weather7Humidity);

	// Save integer parameters
	eeprom_update_word(&alarm1SetEEPROM, received_params.alarm1Set);
	eeprom_update_word(&displayMessage1EEPROM, received_params.displayMessage1);
	eeprom_update_word(&notificationLED1EEPROM, received_params.notificationLED1);
	eeprom_update_word(&buzzer1EEPROM, received_params.buzzer1);
	eeprom_update_word(&hour1EEPROM, received_params.hour1);
	eeprom_update_word(&minute1EEPROM, received_params.minute1);
	eeprom_update_word(&ampm1EEPROM, received_params.ampm1);
	eeprom_update_word(&date1EEPROM, received_params.date1);
	eeprom_update_word(&month1EEPROM, received_params.month1);
	eeprom_update_word(&year1EEPROM, received_params.year1);
	eeprom_update_word(&setAutoEEPROM, received_params.setAuto);
	eeprom_update_word(&alarm2SetEEPROM, received_params.alarm2Set);
	eeprom_update_word(&displayMessage2EEPROM, received_params.displayMessage2);
	eeprom_update_word(&notificationLED2EEPROM, received_params.notificationLED2);
	eeprom_update_word(&buzzer2EEPROM, received_params.buzzer2);
	eeprom_update_word(&hour2EEPROM, received_params.hour2);
	eeprom_update_word(&minute2EEPROM, received_params.minute2);
	eeprom_update_word(&ampm2EEPROM, received_params.ampm2);
	eeprom_update_word(&date2EEPROM, received_params.date2);
	eeprom_update_word(&month2EEPROM, received_params.month2);
	eeprom_update_word(&year2EEPROM, received_params.year2);
	eeprom_update_word(&valueUSBOpticEEPROM, received_params.valueUSBOptic);
	eeprom_update_word(&value12_24EEPROM, received_params.value12_24);
	eeprom_update_word(&ampmEEPROM, received_params.ampm);
	eeprom_update_word(&hourEEPROM, received_params.hour);
	eeprom_update_word(&minuteEEPROM, received_params.minute);
	eeprom_update_word(&dayEEPROM, received_params.day);
	eeprom_update_word(&monthEEPROM, received_params.month);
	eeprom_update_word(&yearEEPROM, received_params.year);
}
 

void load_data_from_EEPROM() {
	// Load string parameters
	eeprom_read_block((void *)received_params.message1, (const void *)message1EEPROM, sizeof(received_params.message1));
	received_params.message1[sizeof(received_params.message1) - 1] = '\0';

	eeprom_read_block((void*)received_params.message2, (const void*)message2EEPROM, sizeof(received_params.message2));
	received_params.message2[sizeof(received_params.message2) - 1] = '\0';

	eeprom_read_block((void*)received_params.weather1Date, (const void*)weather1DateEEPROM, sizeof(received_params.weather1Date));
	received_params.weather1Date[sizeof(received_params.weather1Date) - 1] = '\0';

	eeprom_read_block((void*)received_params.weather2Date, (const void*)weather2DateEEPROM, sizeof(received_params.weather2Date));
	received_params.weather2Date[sizeof(received_params.weather2Date) - 1] = '\0';

	// Repeat the above for the other weather forecast dates
	eeprom_read_block((void*)received_params.weather3Date, (const void*)weather3DateEEPROM, sizeof(received_params.weather3Date));
	received_params.weather3Date[sizeof(received_params.weather3Date) - 1] = '\0';

	eeprom_read_block((void*)received_params.weather4Date, (const void*)weather4DateEEPROM, sizeof(received_params.weather4Date));
	received_params.weather4Date[sizeof(received_params.weather4Date) - 1] = '\0';

	eeprom_read_block((void*)received_params.weather5Date, (const void*)weather5DateEEPROM, sizeof(received_params.weather5Date));
	received_params.weather5Date[sizeof(received_params.weather5Date) - 1] = '\0';

	eeprom_read_block((void*)received_params.weather6Date, (const void*)weather6DateEEPROM, sizeof(received_params.weather6Date));
	received_params.weather6Date[sizeof(received_params.weather6Date) - 1] = '\0';

	eeprom_read_block((void*)received_params.weather7Date, (const void*)weather7DateEEPROM, sizeof(received_params.weather7Date));
	received_params.weather7Date[sizeof(received_params.weather7Date) - 1] = '\0';

	eeprom_read_block((void*)received_params.weather1Forecast, (const void*)weather1ForecastEEPROM, sizeof(received_params.weather1Forecast));
	received_params.weather1Forecast[sizeof(received_params.weather1Forecast) - 1] = '\0';

	eeprom_read_block((void*)received_params.weather2Forecast, (const void*)weather2ForecastEEPROM, sizeof(received_params.weather2Forecast));
	received_params.weather2Forecast[sizeof(received_params.weather2Forecast) - 1] = '\0';

	eeprom_read_block((void*)received_params.weather3Forecast, (const void*)weather3ForecastEEPROM, sizeof(received_params.weather3Forecast));
	received_params.weather3Forecast[sizeof(received_params.weather3Forecast) - 1] = '\0';

	eeprom_read_block((void*)received_params.weather4Forecast, (const void*)weather4ForecastEEPROM, sizeof(received_params.weather4Forecast));
	received_params.weather4Forecast[sizeof(received_params.weather4Forecast) - 1] = '\0';

	eeprom_read_block((void*)received_params.weather5Forecast, (const void*)weather5ForecastEEPROM, sizeof(received_params.weather5Forecast));
	received_params.weather5Forecast[sizeof(received_params.weather5Forecast) - 1] = '\0';

	eeprom_read_block((void*)received_params.weather6Forecast, (const void*)weather6ForecastEEPROM, sizeof(received_params.weather6Forecast));
	received_params.weather6Forecast[sizeof(received_params.weather6Forecast) - 1] = '\0';

	eeprom_read_block((void*)received_params.weather7Forecast, (const void*)weather7ForecastEEPROM, sizeof(received_params.weather7Forecast));
	received_params.weather7Forecast[sizeof(received_params.weather7Forecast) - 1] = '\0';

	// Load float parameters for weather data
	received_params.weather1Temp = eeprom_read_float(&weather1TempEEPROM);
	received_params.weather1Humidity = eeprom_read_float(&weather1HumidityEEPROM);
	received_params.weather2Temp = eeprom_read_float(&weather2TempEEPROM);
	received_params.weather2Humidity = eeprom_read_float(&weather2HumidityEEPROM);
	received_params.weather3Temp = eeprom_read_float(&weather3TempEEPROM);
	received_params.weather3Humidity = eeprom_read_float(&weather3HumidityEEPROM);
	received_params.weather4Temp = eeprom_read_float(&weather4TempEEPROM);
	received_params.weather4Humidity = eeprom_read_float(&weather4HumidityEEPROM);
	received_params.weather5Temp = eeprom_read_float(&weather5TempEEPROM);
	received_params.weather5Humidity = eeprom_read_float(&weather5HumidityEEPROM);
	received_params.weather6Temp = eeprom_read_float(&weather6TempEEPROM);
	received_params.weather6Humidity = eeprom_read_float(&weather6HumidityEEPROM);
	received_params.weather7Temp = eeprom_read_float(&weather7TempEEPROM);
	received_params.weather7Humidity = eeprom_read_float(&weather7HumidityEEPROM);

	// Load integer parameters for alarm 1 and 2
	received_params.alarm1Set = eeprom_read_byte((const uint8_t*)&alarm1SetEEPROM);
	received_params.displayMessage1 = eeprom_read_word(&displayMessage1EEPROM);
	received_params.notificationLED1 = eeprom_read_word(&notificationLED1EEPROM);
	received_params.buzzer1 = eeprom_read_word(&buzzer1EEPROM);
	received_params.hour1 = eeprom_read_word(&hour1EEPROM);
	received_params.minute1 = eeprom_read_word(&minute1EEPROM);
	received_params.ampm1 = eeprom_read_word(&ampm1EEPROM);
	received_params.date1 = eeprom_read_word(&date1EEPROM);
	received_params.month1 = eeprom_read_word(&month1EEPROM);
	received_params.year1 = eeprom_read_word(&year1EEPROM);

	received_params.alarm2Set = eeprom_read_byte((const uint8_t*)&alarm2SetEEPROM);
	received_params.displayMessage2 = eeprom_read_word(&displayMessage2EEPROM);
	received_params.notificationLED2 = eeprom_read_word(&notificationLED2EEPROM);
	received_params.buzzer2 = eeprom_read_word(&buzzer2EEPROM);
	received_params.hour2 = eeprom_read_word(&hour2EEPROM);
	received_params.minute2 = eeprom_read_word(&minute2EEPROM);
	received_params.ampm2 = eeprom_read_word(&ampm2EEPROM);
	received_params.date2 = eeprom_read_word(&date2EEPROM);
	received_params.month2 = eeprom_read_word(&month2EEPROM);
	received_params.year2 = eeprom_read_word(&year2EEPROM);

	// Load system configuration parameters
	received_params.valueUSBOptic = eeprom_read_word(&valueUSBOpticEEPROM);
	received_params.value12_24 = eeprom_read_word(&value12_24EEPROM);
	received_params.ampm = eeprom_read_word(&ampmEEPROM);
	received_params.hour = eeprom_read_word(&hourEEPROM);
	received_params.minute = eeprom_read_word(&minuteEEPROM);
	received_params.day = eeprom_read_word(&dayEEPROM);
	received_params.month = eeprom_read_word(&monthEEPROM);
	received_params.year = eeprom_read_word(&yearEEPROM);

	// Load Celsius/Fahrenheit preference
	received_params.celsius_fahrenheit = eeprom_read_byte(&celsiusFahrenheitEEPROM);
}





void send_data_to_gui() {
	//char buffer[500];  // Buffer to hold the formatted string
	char buffer2[250];

 
	snprintf(buffer2, sizeof(buffer2), "mesage1%s,  notification led%u, 24hour vale %u, am pm value (0 if am  1 if pm:%u, hour:%u, minute %u day%u month: %u, year %u",received_params.message1, received_params.notificationLED1, received_params.value12_24, received_params.ampm, received_params.hour, received_params.minute, received_params.day, received_params.month, received_params.year);
	
	// Format the parameters into a string without labels
	/*snprintf(buffer, sizeof(buffer),
	 "%s %s %u %s %s %s %s %s %s %s %s %s %s %s %s %s %s %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %u %u %u %u %u %u %u %u %u %u "
	 "%u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u",
	received_params.message1, received_params.message2, received_params.celsius_fahrenheit,
	received_params.weather1Date, received_params.weather2Date, received_params.weather3Date, received_params.weather4Date,
	received_params.weather5Date, received_params.weather6Date, received_params.weather7Date,
	received_params.weather1Forecast, received_params.weather2Forecast, received_params.weather3Forecast,
	received_params.weather4Forecast, received_params.weather5Forecast, received_params.weather6Forecast, received_params.weather7Forecast,
	received_params.weather1Temp, received_params.weather1Humidity, received_params.weather2Temp, received_params.weather2Humidity,
	received_params.weather3Temp, received_params.weather3Humidity, received_params.weather4Temp, received_params.weather4Humidity,
	received_params.weather5Temp, received_params.weather5Humidity, received_params.weather6Temp, received_params.weather6Humidity,
	received_params.weather7Temp, received_params.weather7Humidity,
	received_params.alarm1Set, received_params.displayMessage1, received_params.notificationLED1, received_params.buzzer1,
	received_params.hour1, received_params.minute1, received_params.ampm1, received_params.date1, received_params.month1, received_params.year1,
	received_params.alarm2Set, received_params.displayMessage2, received_params.notificationLED2, received_params.buzzer2,
	received_params.hour2, received_params.minute2, received_params.ampm2, received_params.date2, received_params.month2, received_params.year2,
	received_params.valueUSBOptic, received_params.value12_24, received_params.ampm, received_params.hour, received_params.minute,
	received_params.day, received_params.month, received_params.year
	);
*/
	// Send the formatted string over UART
	uart_send_string(buffer2);
	//uart_send_char('\n');  // Send a newline character to mark the end of the transmission
}


/*
void send_eeprom_data_to_gui() {
	char buffer[50];  // Buffer to hold the formatted string

	// Load the data from EEPROM
	load_data_from_EEPROM();

	// Format the EEPROM data into a string
	snprintf(buffer, sizeof(buffer),
	"%s,%s,%u,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u",
	received_params.message1, received_params.message2, received_params.celsius_fahrenheit,
	received_params.weather1Date, received_params.weather2Date, received_params.weather3Date, received_params.weather4Date, received_params.weather5Date, received_params.weather6Date, received_params.weather7Date,
	received_params.weather1Forecast, received_params.weather2Forecast, received_params.weather3Forecast, received_params.weather4Forecast, received_params.weather5Forecast, received_params.weather6Forecast, received_params.weather7Forecast,
	received_params.weather1Temp, received_params.weather1Humidity, received_params.weather2Temp, received_params.weather2Humidity, received_params.weather3Temp, received_params.weather3Humidity, received_params.weather4Temp, received_params.weather4Humidity,
	received_params.weather5Temp, received_params.weather5Humidity, received_params.weather6Temp, received_params.weather6Humidity, received_params.weather7Temp, received_params.weather7Humidity,
	received_params.alarm1Set, received_params.displayMessage1, received_params.notificationLED1, received_params.buzzer1, received_params.hour1, received_params.minute1, received_params.ampm1, received_params.date1, received_params.month1, received_params.year1,
	received_params.alarm2Set, received_params.displayMessage2, received_params.notificationLED2, received_params.buzzer2, received_params.hour2, received_params.minute2, received_params.ampm2, received_params.date2, received_params.month2, received_params.year2,
	received_params.valueUSBOptic, received_params.value12_24, received_params.ampm, received_params.hour, received_params.minute, received_params.day, received_params.month, received_params.year);

	// Send the formatted string over UART
	uart_send_string(buffer);
	uart_send_char('\n');  // Send a newline character to mark the end of the transmission
}
*/

// Function to check connection status with the PC software
int checkConnection() {
	//Send "CHECK" to PC Software
    char buffer[30];
	
	//Print to GUI:
	snprintf(buffer, sizeof(buffer), "CHECK");
	uart_send_string(buffer);
	_delay_ms(50);

    // Wait for "ACK" response from PC Software within the timeout period
	char received_char = ' ';
	char received_string[3];

	for (int i = 0; i < 3; i++) {
		
		if (received_char == '\0') { //uart_receive() timed out
			return 0;
		}
		
		received_char = uart_receive();
		received_string[i] = received_char;
	}
	
	/*
	snprintf(buffer, sizeof(buffer), "received_string: %s", received_string);
	uart_send_string(buffer);
	*/

	if (strcmp(received_string, "ACK") == 0) {
		return 1; // Connected
	} else {
		/*
		snprintf(buffer, sizeof(buffer), "here");
		uart_send_string(buffer);
		*/
		return 0; // If "ACK" isn't received within the timeout period, return disconnected (0)
	}
}

char received_data[600];  // Buffer to hold received data

int main(void) {
	uart_init(9600);  // Initialize UART with 9600 baud rate
	
	   char buffer[600]; 
      char buffer1[32]; 
 
	DDRB = 0xFF;
	PORTB = 0xFF;
	
	
	uint16_t index = 0;  // Index for received data
 
	while (1) {
		// Holds the current connection status
		int connected = 0; // 0 = disconnected, 1 = connected
		
		//CHECK PC CONNECTION AND UPDATE ICON
	    connected = checkConnection(); // Check connection status with the PC

		// Partially update display based on connection status
		char buffer2[30];
		if (connected == 0) { //PC Software is disconnected
			//Print to GUI:
			snprintf(buffer2, sizeof(buffer2), "PC Software is disconnected");
			uart_send_string(buffer2);
			
			// Call display partial update code here
		} else if (connected == 1) { //PC Software is connected
			//Print to GUI:
			snprintf(buffer, sizeof(buffer), "PC Software is connected");
			uart_send_string(buffer);
			
			// Call display partial update code here
		}

		_delay_ms(500); // Delay between updates for stability

		/*
		char received_char = uart_receive();
		snprintf(buffer, sizeof(buffer), "%c ", received_char);
		

		uart_send_string(buffer); // maybe need to check if buffer is full and set to 0
		
			
		if (received_char == '\n') {
            received_data[index] = '\0';  // Null-terminate the string
            index = 0;  // Reset index 
            parse_received_data(received_data);  // Parse into variables
			 
        } else {
            
            if (index < sizeof(received_data) - 1) {
                received_data[index++] = received_char;  // Store character in buffer
					snprintf(buffer1, sizeof(buffer), "size :%d ", index);
					
					uart_send_string(buffer1);
					//	save_data_to_EEPROM();
            }  
        } 
		*/          
	}	
}


 