#include "serial.h"

// Function to initialize UART communication   
void uart_init(unsigned int baud) {
	unsigned int ubrr = F_CPU/16/baud - 1;
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);  // Enable receiver and transmitter
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); // Set frame format: 8 data bits, 1 stop bit
}

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
// Function to receive data via UART
char uart_receive(void) {
	while (!(UCSR0A & (1<<RXC0)));  // Wait for data to be received
	return UDR0;                    // Get and return received data from buffer
}

	
void parse_receive_data(char *received_data) {
	// Use strtok to split the received string based on commas
	char *token;
	//char buffer[800];
	// Parse message1
	/*token = strtok(received_data, ",");
	strncpy(received_params.message1, token, MESSAGE_SIZE);
	received_params.message1[MESSAGE_SIZE - 1] = '\0';  // Ensure null-termination

	// Parse message2
	token = strtok(NULL, ",");
	strncpy(received_params.message2, token, MESSAGE_SIZE);
	received_params.message2[MESSAGE_SIZE - 1] = '\0';  // Ensure null-termination
*/
	token = strtok(received_data, ","); //First token must be this
	// Parse celsius_fahrenheit
	//token = strtok(NULL, ",");
	if (token != NULL) {
		received_params.celsius_fahrenheit = (uint8_t)atoi(token);
	}
	
	
	token = strtok(NULL, ",");
	received_params.hour = (uint16_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.minute = (uint16_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.day = (uint16_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.month = (uint16_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.year = (uint16_t)atoi(token);
	// Parse weather dates
	// Parse weather data
	for (int i = 0; i < 7; i++) {
		// Date
		token = strtok(NULL, ",");
		if (token == NULL) break;  // Exit if no more tokens
		switch (i) {
			case 0: strncpy(received_params.weather1Date, token, WEATHER_DATE_SIZE); break;
			case 1: strncpy(received_params.weather2Date, token, WEATHER_DATE_SIZE); break;
			case 2: strncpy(received_params.weather3Date, token, WEATHER_DATE_SIZE); break;
			case 3: strncpy(received_params.weather4Date, token, WEATHER_DATE_SIZE); break;
			case 4: strncpy(received_params.weather5Date, token, WEATHER_DATE_SIZE); break;
			case 5: strncpy(received_params.weather6Date, token, WEATHER_DATE_SIZE); break;
			case 6: strncpy(received_params.weather7Date, token, WEATHER_DATE_SIZE); break;
		}
		received_params.weather1Date[WEATHER_DATE_SIZE - 1] = '\0';  // Ensure null-termination
	}

	// Parse forecast data
	for (int i = 0; i < 7; i++) {
		token = strtok(NULL, ",");
		if (token == NULL) break;  // Exit if no more tokens
		switch (i) {
			case 0: strncpy(received_params.weather1Forecast, token, FORECAST_SIZE); break;
			case 1: strncpy(received_params.weather2Forecast, token, FORECAST_SIZE); break;
			case 2: strncpy(received_params.weather3Forecast, token, FORECAST_SIZE); break;
			case 3: strncpy(received_params.weather4Forecast, token, FORECAST_SIZE); break;
			case 4: strncpy(received_params.weather5Forecast, token, FORECAST_SIZE); break;
			case 5: strncpy(received_params.weather6Forecast, token, FORECAST_SIZE); break;
			case 6: strncpy(received_params.weather7Forecast, token, FORECAST_SIZE); break;
		}
		received_params.weather1Forecast[FORECAST_SIZE - 1] = '\0';  // Ensure null-termination
	}

	// Parse temperature and humidity data
	for (int i = 0; i < 7; i++) {
		// Temperature
		token = strtok(NULL, ",");
		if (token == NULL) break;  // Exit if no more tokens
		switch (i) {
			case 0: received_params.weather1Temp = (uint16_t)atoi(token); break;
			case 1: received_params.weather2Temp = (uint16_t)atoi(token); break;
			case 2: received_params.weather3Temp = (uint16_t)atoi(token); break;
			case 3: received_params.weather4Temp = (uint16_t)atoi(token); break;
			case 4: received_params.weather5Temp = (uint16_t)atoi(token); break;
			case 5: received_params.weather6Temp = (uint16_t)atoi(token); break;
			case 6: received_params.weather7Temp = (uint16_t)atoi(token); break;
		}

		// Humidity
		token = strtok(NULL, ",");
		if (token == NULL) break;  // Exit if no more tokens
		switch (i) {
			case 0: received_params.weather1Humidity = (uint16_t)atoi(token); break;
			case 1: received_params.weather2Humidity = (uint16_t)atoi(token); break;
			case 2: received_params.weather3Humidity = (uint16_t)atoi(token); break;
			case 3: received_params.weather4Humidity = (uint16_t)atoi(token); break;
			case 4: received_params.weather5Humidity = (uint16_t)atoi(token); break;
			case 5: received_params.weather6Humidity = (uint16_t)atoi(token); break;
			case 6: received_params.weather7Humidity = (uint16_t)atoi(token); break;
		}
	}

	// Parse alarm1 settings
	token = strtok(NULL, ",");
	received_params.alarm1Set = (uint8_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.displayMessage1 = (uint16_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.notificationLED1 = (uint16_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.buzzer1 = (uint16_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.hour1 = (uint16_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.minute1 = (uint16_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.ampm1 = (uint16_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.date1 = (uint16_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.month1 = (uint16_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.year1 = (uint16_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.setAuto = (uint16_t)atoi(token);

	// Parse alarm2 settings
	token = strtok(NULL, ",");
	received_params.alarm2Set = (uint8_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.displayMessage2 = (uint16_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.notificationLED2 = (uint16_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.buzzer2 = (uint16_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.hour2 = (uint16_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.minute2 = (uint16_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.ampm2 = (uint16_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.date2 = (uint16_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.month2 = (uint16_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.year2 = (uint16_t)atoi(token);

	// Parse other settings
	token = strtok(NULL, ",");
	received_params.valueUSBOptic = (uint16_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.value12_24 = (uint16_t)atoi(token);

	token = strtok(NULL, ",");
	received_params.ampm = (uint16_t)atoi(token);
/*

	// Format the parsed data into a string
	snprintf(buffer, sizeof(buffer),
	"  celsius_fahrenheit: %u, "
	"weather1Date: %s, weather1Forecast: %s, humid:%u"
	"hour: %u, minute: %u, day: %u, month: %u, year: %u, Temperature: :%u alarm1 is set: %u, alarm date %u, alarm year%u ",

	received_params.celsius_fahrenheit,
	received_params.weather1Date,
	received_params.weather1Forecast,
	received_params.weather1Humidity,
	received_params.hour,
	received_params.minute,
	received_params.day,
	received_params.month,
	received_params.year,
	received_params.weather1Temp,
	received_params.alarm1Set,
	received_params.date1,
	received_params.year1 
	);

	// Send the formatted string back via UART
	uart_send_string(buffer); */
//	send_data_to_gui();
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

	// Save uint16_t parameters (weather data)
	eeprom_update_word(&weather1TempEEPROM, received_params.weather1Temp);
	eeprom_update_word(&weather1HumidityEEPROM, received_params.weather1Humidity);
	eeprom_update_word(&weather2TempEEPROM, received_params.weather2Temp);
	eeprom_update_word(&weather2HumidityEEPROM, received_params.weather2Humidity);
	eeprom_update_word(&weather3TempEEPROM, received_params.weather3Temp);
	eeprom_update_word(&weather3HumidityEEPROM, received_params.weather3Humidity);
	eeprom_update_word(&weather4TempEEPROM, received_params.weather4Temp);
	eeprom_update_word(&weather4HumidityEEPROM, received_params.weather4Humidity);
	eeprom_update_word(&weather5TempEEPROM, received_params.weather5Temp);
	eeprom_update_word(&weather5HumidityEEPROM, received_params.weather5Humidity);
	eeprom_update_word(&weather6TempEEPROM, received_params.weather6Temp);
	eeprom_update_word(&weather6HumidityEEPROM, received_params.weather6Humidity);
	eeprom_update_word(&weather7TempEEPROM, received_params.weather7Temp);
	eeprom_update_word(&weather7HumidityEEPROM, received_params.weather7Humidity);

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

	// Load uint16_t parameters for weather data
	received_params.weather1Temp = eeprom_read_word(&weather1TempEEPROM);
	received_params.weather1Humidity = eeprom_read_word(&weather1HumidityEEPROM);
	received_params.weather2Temp = eeprom_read_word(&weather2TempEEPROM);
	received_params.weather2Humidity = eeprom_read_word(&weather2HumidityEEPROM);
	received_params.weather3Temp = eeprom_read_word(&weather3TempEEPROM);
	received_params.weather3Humidity = eeprom_read_word(&weather3HumidityEEPROM);
	received_params.weather4Temp = eeprom_read_word(&weather4TempEEPROM);
	received_params.weather4Humidity = eeprom_read_word(&weather4HumidityEEPROM);
	received_params.weather5Temp = eeprom_read_word(&weather5TempEEPROM);
	received_params.weather5Humidity = eeprom_read_word(&weather5HumidityEEPROM);
	received_params.weather6Temp = eeprom_read_word(&weather6TempEEPROM);
	received_params.weather6Humidity = eeprom_read_word(&weather6HumidityEEPROM);
	received_params.weather7Temp = eeprom_read_word(&weather7TempEEPROM);
	received_params.weather7Humidity = eeprom_read_word(&weather7HumidityEEPROM);

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
	
	/*
	char buffer[800];
	
	// Format the parsed data into a string
	snprintf(buffer, sizeof(buffer),
	" DATA FROM EEPROM: celsius_fahrenheit: %u, "
	"weather1Date: %s, weather1Forecast: %s, humid:%u"
	"hour: %u, minute: %u, day: %u, month: %u, year: %u, Temperature: :%u alarm1 is set: %u, alarm date %u, alarm year%u ",
	
	received_params.celsius_fahrenheit,
	received_params.weather1Date,
	received_params.weather1Forecast,
	received_params.weather1Humidity,
	received_params.hour,
	received_params.minute,
	received_params.day,
	received_params.month,
	received_params.year,
	received_params.weather1Temp,
	received_params.alarm1Set,
	received_params.date1,
	received_params.year1
	);

	// Send the formatted string back via UART
	uart_send_string(buffer);
	*/
	send_EEPROM_data();
	
}

void send_EEPROM_data() {
	char buffer[800];   

	snprintf(buffer, sizeof(buffer),
		"DATA FROM EEPROM:\n"
		"Celsius/Fahrenheit: %u\n"
		"Weather1 Date: %s, Forecast: %s, Temp: %u, Humidity: %u\n"
		"Weather2 Date: %s, Forecast: %s, Temp: %u, Humidity: %u\n"
		"Weather3 Date: %s, Forecast: %s, Temp: %u, Humidity: %u\n"
		"Weather4 Date: %s, Forecast: %s, Temp: %u, Humidity: %u\n"
		"Weather5 Date: %s, Forecast: %s, Temp: %u, Humidity: %u\n"
		"Weather6 Date: %s, Forecast: %s, Temp: %u, Humidity: %u\n"
		"Weather7 Date: %s, Forecast: %s, Temp: %u, Humidity: %u\n"
		"Alarm1 Set: %u, Message: %u, LED: %u, Buzzer: %u, "
		"Hour: %u, Minute: %u, AM/PM: %u, Date: %u, Month: %u, Year: %u\n"
		"Alarm2 Set: %u, Message: %u, LED: %u, Buzzer: %u, "
		"Hour: %u, Minute: %u, AM/PM: %u, Date: %u, Month: %u, Year: %u\n"
		"System Time - Hour: %u, Minute: %u, Day: %u, Month: %u, Year: %u\n"
		"Value USB Optic: %u\n"
		"12/24 Hour Format: %u\n"
		"AM/PM Indicator: %u\n",

		// Will need to add message data herw
		received_params.celsius_fahrenheit,
		received_params.weather1Date, received_params.weather1Forecast, received_params.weather1Temp, received_params.weather1Humidity,
		received_params.weather2Date, received_params.weather2Forecast, received_params.weather2Temp, received_params.weather2Humidity,
		received_params.weather3Date, received_params.weather3Forecast, received_params.weather3Temp, received_params.weather3Humidity,
		received_params.weather4Date, received_params.weather4Forecast, received_params.weather4Temp, received_params.weather4Humidity,
		received_params.weather5Date, received_params.weather5Forecast, received_params.weather5Temp, received_params.weather5Humidity,
		received_params.weather6Date, received_params.weather6Forecast, received_params.weather6Temp, received_params.weather6Humidity,
		received_params.weather7Date, received_params.weather7Forecast, received_params.weather7Temp, received_params.weather7Humidity,
		received_params.alarm1Set, received_params.displayMessage1, received_params.notificationLED1, received_params.buzzer1,
		received_params.hour1, received_params.minute1, received_params.ampm1, received_params.date1, received_params.month1, received_params.year1,
		received_params.alarm2Set, received_params.displayMessage2, received_params.notificationLED2, received_params.buzzer2,
		received_params.hour2, received_params.minute2, received_params.ampm2, received_params.date2, received_params.month2, received_params.year2,
		received_params.hour, received_params.minute, received_params.day, received_params.month, received_params.year,
		received_params.valueUSBOptic,
		received_params.value12_24,
		received_params.ampm
	);

	uart_send_string(buffer);
}




/*
void send_data_to_gui() {
	//char buffer[500];  // Buffer to hold the formatted string
	char buffer2[250];


	snprintf(buffer2, sizeof(buffer2), "message1: %s, notification led: %u, 24-hour format: %u, am/pm: %u, hour: %u, minute: %u, day: %u, month: %u, year: %u",
	received_params.message1, received_params.notificationLED1, received_params.value12_24, received_params.ampm, received_params.hour,
	received_params.minute, received_params.day, received_params.month, received_params.year);
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

	// Send the formatted string over UART
	uart_send_string(buffer2);
	//uart_send_char('\n');  // Send a newline character to mark the end of the transmission
}
*/

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

char received_data[800];  // Buffer to hold received data



int main(void) {
	uart_init(9600);  // Initialize UART with 9600 baud rate
	
	//  char buffer[600]; 
	//char buffer1[32]; 

	DDRB = 0xFF;
	PORTB = 0xFF;
	
	
	uint16_t index = 0;  // Index for received data

	while (1) {
		char received_char = uart_receive();
		//snprintf(buffer, sizeof(buffer), "%c ", received_char);
		

	//	uart_send_string(buffer); // maybe need to check if buffer is full and set to 0
		
			
		if (received_char == '\n') {
			received_data[index] = '\0';  // Null-terminate the string
			
			
			//snprintf(buffer, sizeof(buffer), "received: %s", received_data); USE THIS FOR DEBUG
			
			//uart_send_string(buffer);
			index = 0;  // Reset index 
			parse_receive_data(received_data);  // Parse into variables
			save_data_to_EEPROM();
			load_data_from_EEPROM();
			
		} else {
			
			if (index < sizeof(received_data) - 1) {
				received_data[index++] = received_char;  // Store character in buffer
					
					
			}  
		}           
		
		
	}
}


