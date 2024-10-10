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

// Allocating EEPROM memory with EEMEM
char EEMEM message1EEPROM[257]; // 256 characters + null terminator
char EEMEM message2EEPROM[257];
uint8_t EEMEM celsiusFahrenheitEEPROM;

char EEMEM weather1DateEEPROM[10];
char EEMEM weather2DateEEPROM[10];
char EEMEM weather3DateEEPROM[10];
char EEMEM weather4DateEEPROM[10];
char EEMEM weather5DateEEPROM[10];
char EEMEM weather6DateEEPROM[10];
char EEMEM weather7DateEEPROM[10];

char EEMEM weather1ForecastEEPROM[10];
char EEMEM weather2ForecastEEPROM[10];
char EEMEM weather3ForecastEEPROM[10];
char EEMEM weather4ForecastEEPROM[10];
char EEMEM weather5ForecastEEPROM[10];
char EEMEM weather6ForecastEEPROM[10];
char EEMEM weather7ForecastEEPROM[10];

// allocate EEPROM for float parameters...
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

// Allocating EEPROM for integer parameters
uint8_t EEMEM alarm1SetEEPROM;
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
uint8_t EEMEM alarm2SetEEPROM;
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

/*
struct Parameters {
    // Declare the string parameters array
    char string_parameters[NO_OF_STRING_PARAMETERS][15];
    // Declare the float parameters array
    float float_parameters[NO_OF_FLOAT_PARAMETERS];
    // Declare the integer parameters array
    int int_parameters[NO_OF_INT_PARAMETERS];
};
*/

// Received parameters
// (String parameters)
char* message1;
char* message2;
char celsiusFahrenheit;
char* weather1Date;
char* weather2Date;
char* weather3Date;
char* weather4Date;
char* weather5Date;
char* weather6Date;
char* weather7Date;

char* weather1Forecast;
char* weather2Forecast;
char* weather3Forecast;
char* weather4Forecast;
char* weather5Forecast;
char* weather6Forecast;
char* weather7Forecast;

// (Float parameters)
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

// (Integer parameters)
int alarm1Set;
int displayMessage1;
int notificationLED1;
int buzzer1;
int hour1;
int minute1;
int ampm1;
int date1;
int month1;
int year1;

int alarm2Set;
int displayMessage2;
int notificationLED2;
int buzzer2;
int hour2;
int minute2;
int ampm2;
int date2;
int month2;
int year2;

int setAuto;
int valueUSBOptic;
int value12_24;
int ampm;
int hour;
int minute;
int day;
int month;
int year;

// Declare functions
void uart_init(unsigned int baud);
void uart_send(char data);
char uart_receive(void);
void USART_send_char(char c);
void USART_send_string(const char* str);
void save_data_to_EEPROM();
void load_data_from_EEPROM();
void receive_parameters();
void output_parameters();

int main(void) {

	
	uart_init(9600);  // Initialize UART with 9600 baud rate
	
	/*
	// Testing print functionality
	snprintf(buffer, sizeof(buffer), "here");
    USART_send_string(buffer);
	*/
	
	DDRB = 0xFF;
	PORTB = 0xFF;

	
	
	while (1) {
		/*
		char received = uart_receive();  // Receive data via UART
		uart_send(received);  // Send back the received data
		*/
		
		//RECEIVE PARAMETERS FROM GUI AND STORE IN VARIABLES
		receive_parameters();
		//will need to move into the while loop once using real received data (once received, write parameters to EEPROM)
		save_data_to_EEPROM();
		//output_parameters(); // Print all the parameters out
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

void USART_send_char(char c) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

void USART_send_string(const char* str) {
    while (*str) {
        USART_send_char(*str++);
    }
}

// Updates parameters in EEPROM with new parameter values if required
void save_data_to_EEPROM() {

    // Alarm messages
    eeprom_update_block((const void *)message1, (void *)message1EEPROM, 257);
    eeprom_update_block((const void *)message2, (void *)message2EEPROM, 257);

    // Celsius/Fahrenheit setting
    eeprom_update_byte((uint8_t*)&celsiusFahrenheitEEPROM, celsiusFahrenheit);

    // Weather Dates
    eeprom_update_block((const void*)weather1Date, (void*)weather1DateEEPROM, 10);
    eeprom_update_block((const void*)weather2Date, (void*)weather2DateEEPROM, 10);
    eeprom_update_block((const void*)weather3Date, (void*)weather3DateEEPROM, 10);
    eeprom_update_block((const void*)weather4Date, (void*)weather4DateEEPROM, 10);
    eeprom_update_block((const void*)weather5Date, (void*)weather5DateEEPROM, 10);
    eeprom_update_block((const void*)weather6Date, (void*)weather6DateEEPROM, 10);
    eeprom_update_block((const void*)weather7Date, (void*)weather7DateEEPROM, 10);

    // Weather Forecasts
    eeprom_update_block((const void*)weather1Forecast, (void*)weather1ForecastEEPROM, 10);
    eeprom_update_block((const void*)weather2Forecast, (void*)weather2ForecastEEPROM, 10);
    eeprom_update_block((const void*)weather3Forecast, (void*)weather3ForecastEEPROM, 10);
    eeprom_update_block((const void*)weather4Forecast, (void*)weather4ForecastEEPROM, 10);
    eeprom_update_block((const void*)weather5Forecast, (void*)weather5ForecastEEPROM, 10);
    eeprom_update_block((const void*)weather6Forecast, (void*)weather6ForecastEEPROM, 10);
    eeprom_update_block((const void*)weather7Forecast, (void*)weather7ForecastEEPROM, 10);

    // Alarm 1
    eeprom_update_byte((uint8_t*)&alarm1SetEEPROM, alarm1Set);
    eeprom_update_block(&displayMessage1, &displayMessage1EEPROM, sizeof(displayMessage1));
    eeprom_update_block(&notificationLED1, &notificationLED1EEPROM, sizeof(notificationLED1));
    eeprom_update_block(&buzzer1, &buzzer1EEPROM, sizeof(buzzer1));
    eeprom_update_block(&hour1, &hour1EEPROM, sizeof(hour1));
    eeprom_update_block(&minute1, &minute1EEPROM, sizeof(minute1));
    eeprom_update_block(&ampm1, &ampm1EEPROM, sizeof(ampm1));
    eeprom_update_block(&date1, &date1EEPROM, sizeof(date1));
    eeprom_update_block(&month1, &month1EEPROM, sizeof(month1));
    eeprom_update_block(&year1, &year1EEPROM, sizeof(year1));
    eeprom_update_byte((uint8_t*)&setAutoEEPROM, setAuto);

    // Alarm 2
    eeprom_update_byte((uint8_t*)&alarm2SetEEPROM, alarm2Set);
    eeprom_update_block(&displayMessage2, &displayMessage2EEPROM, sizeof(displayMessage2));
    eeprom_update_block(&notificationLED2, &notificationLED2EEPROM, sizeof(notificationLED2));
    eeprom_update_block(&buzzer2, &buzzer2EEPROM, sizeof(buzzer2));
    eeprom_update_block(&hour2, &hour2EEPROM, sizeof(hour2));
    eeprom_update_block(&minute2, &minute2EEPROM, sizeof(minute2));
    eeprom_update_block(&ampm2, &ampm2EEPROM, sizeof(ampm2));
    eeprom_update_block(&date2, &date2EEPROM, sizeof(date2));
    eeprom_update_block(&month2, &month2EEPROM, sizeof(month2));
    eeprom_update_block(&year2, &year2EEPROM, sizeof(year2));

    // System params
    eeprom_update_block(&valueUSBOptic, &valueUSBOpticEEPROM, sizeof(valueUSBOptic));
    eeprom_update_block(&value12_24, &value12_24EEPROM, sizeof(value12_24));
    eeprom_update_block(&ampm, &ampmEEPROM, sizeof(ampm));
    eeprom_update_block(&hour, &hourEEPROM, sizeof(hour));
    eeprom_update_block(&minute, &minuteEEPROM, sizeof(minute));
    eeprom_update_block(&day, &dayEEPROM, sizeof(day));
    eeprom_update_block(&month, &monthEEPROM, sizeof(month));
    eeprom_update_block(&year, &yearEEPROM, sizeof(year));

    // Weather Temperature and Humidity (floats)
    eeprom_update_float(&weather1TempEEPROM, weather1Temp);
    eeprom_update_float(&weather1HumidityEEPROM, weather1Humidity);
    eeprom_update_float(&weather2TempEEPROM, weather2Temp);
    eeprom_update_float(&weather2HumidityEEPROM, weather2Humidity);
    eeprom_update_float(&weather3TempEEPROM, weather3Temp);
    eeprom_update_float(&weather3HumidityEEPROM, weather3Humidity);
    eeprom_update_float(&weather4TempEEPROM, weather4Temp);
    eeprom_update_float(&weather4HumidityEEPROM, weather4Humidity);
    eeprom_update_float(&weather5TempEEPROM, weather5Temp);
    eeprom_update_float(&weather5HumidityEEPROM, weather5Humidity);
    eeprom_update_float(&weather6TempEEPROM, weather6Temp);
    eeprom_update_float(&weather6HumidityEEPROM, weather6Humidity);
    eeprom_update_float(&weather7TempEEPROM, weather7Temp);
    eeprom_update_float(&weather7HumidityEEPROM, weather7Humidity);
	
	// Clear variables so that we can see what load_data_from_EEPROM() sets the variables to
	// (String parameters)
	message1 = "";
	message2 = "";
	celsiusFahrenheit = ' ';
	weather1Date = "";
	weather2Date = "";
	weather3Date = "";
	weather4Date = "";
	weather5Date = "";
	weather6Date = "";
	weather7Date = "";

	weather1Forecast = "";
	weather2Forecast = "";
	weather3Forecast = "";
	weather4Forecast = "";
	weather5Forecast = "";
	weather6Forecast = "";
	weather7Forecast = "";

	// (Float parameters)
	weather1Temp = 0.f;
	weather1Humidity = 0.f;
	weather2Temp = 0.f;
	weather2Humidity = 0.f;
	weather3Temp = 0.f;
	weather3Humidity = 0.f;
	weather4Temp = 0.f;
	weather4Humidity = 0.f;
	weather5Temp = 0.f;
	weather5Humidity = 0.f;
	weather6Temp = 0.f;
	weather6Humidity = 0.f;
	weather7Temp = 0.f;
	weather7Humidity = 0.f;

	// (Integer parameters)
	alarm1Set = 0;
	displayMessage1 = 0;
	notificationLED1 = 0;
	buzzer1 = 0;
	hour1 = 0;
	minute1 = 0;
	ampm1 = 0;
	date1 = 0;
	month1 = 0;
	year1 = 0;

	alarm2Set = 0;
	displayMessage2 = 0;
	notificationLED2 = 0;
	buzzer2 = 0;
	hour2 = 0;
	minute2 = 0;
	ampm2 = 0;
	date2 = 0;
	month2 = 0;
	year2 = 0;

	setAuto = 0;
	valueUSBOptic = 0;
	value12_24 = 0;
	ampm = 0;
	hour = 0;
	minute = 0;
	day = 0;
	month = 0;
	year = 0;
}

//Load parameters from EEPROM address to SRAM address
void load_data_from_EEPROM() {
    // Load string parameters
    eeprom_read_block((void *)message1, (const void *)message1EEPROM, sizeof(message1));
    message1[sizeof(message1) - 1] = '\0';

    eeprom_read_block((void*)message2, (const void*)message2EEPROM, sizeof(message2));
    message2[sizeof(message2) - 1] = '\0';

    eeprom_read_block((void*)weather1Date, (const void*)weather1DateEEPROM, sizeof(weather1Date));
    weather1Date[sizeof(weather1Date) - 1] = '\0';

    eeprom_read_block((void*)weather2Date, (const void*)weather2DateEEPROM, sizeof(weather2Date));
    weather2Date[sizeof(weather2Date) - 1] = '\0';

    // Repeat the above for the other weather forecast dates
    eeprom_read_block((void*)weather3Date, (const void*)weather3DateEEPROM, sizeof(weather3Date));
    weather3Date[sizeof(weather3Date) - 1] = '\0';

    eeprom_read_block((void*)weather4Date, (const void*)weather4DateEEPROM, sizeof(weather4Date));
    weather4Date[sizeof(weather4Date) - 1] = '\0';

    eeprom_read_block((void*)weather5Date, (const void*)weather5DateEEPROM, sizeof(weather5Date));
    weather5Date[sizeof(weather5Date) - 1] = '\0';

    eeprom_read_block((void*)weather6Date, (const void*)weather6DateEEPROM, sizeof(weather6Date));
    weather6Date[sizeof(weather6Date) - 1] = '\0';

    eeprom_read_block((void*)weather7Date, (const void*)weather7DateEEPROM, sizeof(weather7Date));
    weather7Date[sizeof(weather7Date) - 1] = '\0';

    eeprom_read_block((void*)weather1Forecast, (const void*)weather1ForecastEEPROM, sizeof(weather1Forecast));
    weather1Forecast[sizeof(weather1Forecast) - 1] = '\0';

    eeprom_read_block((void*)weather2Forecast, (const void*)weather2ForecastEEPROM, sizeof(weather2Forecast));
    weather2Forecast[sizeof(weather2Forecast) - 1] = '\0';

    eeprom_read_block((void*)weather3Forecast, (const void*)weather3ForecastEEPROM, sizeof(weather3Forecast));
    weather3Forecast[sizeof(weather3Forecast) - 1] = '\0';

    eeprom_read_block((void*)weather4Forecast, (const void*)weather4ForecastEEPROM, sizeof(weather4Forecast));
    weather4Forecast[sizeof(weather4Forecast) - 1] = '\0';

    eeprom_read_block((void*)weather5Forecast, (const void*)weather5ForecastEEPROM, sizeof(weather5Forecast));
    weather5Forecast[sizeof(weather5Forecast) - 1] = '\0';

    eeprom_read_block((void*)weather6Forecast, (const void*)weather6ForecastEEPROM, sizeof(weather6Forecast));
    weather6Forecast[sizeof(weather6Forecast) - 1] = '\0';

    eeprom_read_block((void*)weather7Forecast, (const void*)weather7ForecastEEPROM, sizeof(weather7Forecast));
    weather7Forecast[sizeof(weather7Forecast) - 1] = '\0';

    // Load float parameters for weather data
    weather1Temp = eeprom_read_float(&weather1TempEEPROM);
    weather1Humidity = eeprom_read_float(&weather1HumidityEEPROM);
    weather2Temp = eeprom_read_float(&weather2TempEEPROM);
    weather2Humidity = eeprom_read_float(&weather2HumidityEEPROM);
    weather3Temp = eeprom_read_float(&weather3TempEEPROM);
    weather3Humidity = eeprom_read_float(&weather3HumidityEEPROM);
    weather4Temp = eeprom_read_float(&weather4TempEEPROM);
    weather4Humidity = eeprom_read_float(&weather4HumidityEEPROM);
    weather5Temp = eeprom_read_float(&weather5TempEEPROM);
    weather5Humidity = eeprom_read_float(&weather5HumidityEEPROM);
    weather6Temp = eeprom_read_float(&weather6TempEEPROM);
    weather6Humidity = eeprom_read_float(&weather6HumidityEEPROM);
    weather7Temp = eeprom_read_float(&weather7TempEEPROM);
    weather7Humidity = eeprom_read_float(&weather7HumidityEEPROM);

    // Load integer parameters for alarm 1 and 2
    alarm1Set = eeprom_read_byte(&alarm1SetEEPROM);
    displayMessage1 = eeprom_read_word(&displayMessage1EEPROM);
    notificationLED1 = eeprom_read_word(&notificationLED1EEPROM);
    buzzer1 = eeprom_read_word(&buzzer1EEPROM);
    hour1 = eeprom_read_word(&hour1EEPROM);
    minute1 = eeprom_read_word(&minute1EEPROM);
    ampm1 = eeprom_read_word(&ampm1EEPROM);
    date1 = eeprom_read_word(&date1EEPROM);
    month1 = eeprom_read_word(&month1EEPROM);
    year1 = eeprom_read_word(&year1EEPROM);


    alarm2Set = eeprom_read_byte(&alarm2SetEEPROM);
    displayMessage2 = eeprom_read_word(&displayMessage2EEPROM);
    notificationLED2 = eeprom_read_word(&notificationLED2EEPROM);
    buzzer2 = eeprom_read_word(&buzzer2EEPROM);
    hour2 = eeprom_read_word(&hour2EEPROM);
    minute2 = eeprom_read_word(&minute2EEPROM);
    ampm2 = eeprom_read_word(&ampm2EEPROM);
    date2 = eeprom_read_word(&date2EEPROM);
    month2 = eeprom_read_word(&month2EEPROM);
    year2 = eeprom_read_word(&year2EEPROM);

    // Load system configuration parameters
    valueUSBOptic = eeprom_read_word(&valueUSBOpticEEPROM);
    value12_24 = eeprom_read_word(&value12_24EEPROM);
    ampm = eeprom_read_word(&ampmEEPROM);
    hour = eeprom_read_word(&hourEEPROM);
    minute = eeprom_read_word(&minuteEEPROM);
    day = eeprom_read_word(&dayEEPROM);
    month = eeprom_read_word(&monthEEPROM);
    year = eeprom_read_word(&yearEEPROM);

    // Load Celsius/Fahrenheit preference
    celsiusFahrenheit = eeprom_read_byte(&celsiusFahrenheitEEPROM);
}

// Save received parameters into variables
void receive_parameters() {
    //receive JSON data
    //decode JSON data
    //assign data to variables (using fake data first)

    // (String parameters)
    message1 = "Alarm 1 message";
    message2 = "Alarm 2 message";
    celsiusFahrenheit = 'C';
    weather1Date = "2024-09-20";
    weather2Date = "2024-09-21";
    weather3Date = "2024-09-22";
    weather4Date = "2024-09-23";
    weather5Date = "2024-09-24";
    weather6Date = "2024-09-25";
    weather7Date = "2024-09-26";

    weather1Forecast = "Sunny";
    weather2Forecast = "Cloudy";
    weather3Forecast = "Rainy";
    weather4Forecast = "Windy";
    weather5Forecast = "Stormy";
    weather6Forecast = "Foggy";
    weather7Forecast = "Clear";

    // (Float parameters)
    weather1Temp = 25.123;
    weather1Humidity = 60.456;
    weather2Temp = 24.789;
    weather2Humidity = 65.321;
    weather3Temp = 22.654;
    weather3Humidity = 70.987;
    weather4Temp = 20.111;
    weather4Humidity = 75.222;
    weather5Temp = 18.333;
    weather5Humidity = 80.444;
    weather6Temp = 17.555;
    weather6Humidity = 85.666;
    weather7Temp = 16.777;
    weather7Humidity = 90.888;

    // (Integer parameters)
    alarm1Set = 1;
    displayMessage1 = 0;
    notificationLED1 = 1;
    buzzer1 = 1;
    hour1 = 8;
    minute1 = 30;
    ampm1 = 0;
    date1 = 20;
    month1 = 9;
    year1 = 2024;

    alarm2Set = 1;
    displayMessage2 = 0;
    notificationLED2 = 1;
    buzzer2 = 0;
    hour2 = 0;
    minute2 = 9;
    ampm2 = 0;
    date2 = 21;
    month2 = 9;
    year2 = 2023;

    setAuto = 0;
    valueUSBOptic = 1;
    value12_24 = 0;
    ampm = 10;
    hour = 15;
    minute = 20;
    day = 9;
    month = 9;
    year = 2023;

}

// Prints all the parameters saved to EEPROM
void output_parameters() {
	load_data_from_EEPROM();
	
	char buffer[1000]; // Declare buffer
	
	/*
	// Print out all values loaded from EEPROM
	snprintf(buffer, sizeof(buffer),
	"%s, %s, %c, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i",
	message1, message2, celsiusFahrenheit, weather1Date, weather2Date, weather3Date, weather4Date, weather5Date, weather6Date, weather7Date,
	weather1Forecast, weather2Forecast, weather3Forecast, weather4Forecast, weather5Forecast, weather6Forecast, weather7Forecast,
	weather1Temp, weather1Humidity, weather2Temp, weather2Humidity, weather3Temp, weather3Humidity, weather4Temp, weather4Humidity,
	weather5Temp, weather5Humidity, weather6Temp, weather6Humidity, weather7Temp, weather7Humidity,
	alarm1Set, displayMessage1, notificationLED1, buzzer1, hour1, minute1, ampm1, date1, month1, year1,
	alarm2Set, displayMessage2, notificationLED2, buzzer2, hour2, minute2, ampm2, date2, month2, year2,
	setAuto, valueUSBOptic, value12_24, ampm, hour, minute, day, month, year
	);
	*/
	
	// Print one value from EEPROM
	snprintf(buffer, sizeof(buffer), "%s", message1);

	USART_send_string(buffer);
	
}