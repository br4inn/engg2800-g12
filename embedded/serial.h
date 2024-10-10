#ifndef SERIAL_H
#define SERIAL_H


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
#define WEATHER_DATE_SIZE 11 //YYYY-MM-DD + NULL TERMINATOR?
#define FORECAST_SIZE 20


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
    uint16_t weather1Temp;
    uint16_t weather1Humidity;
    uint16_t weather2Temp;
    uint16_t weather2Humidity;
    uint16_t weather3Temp;
    uint16_t weather3Humidity;
    uint16_t weather4Temp;
    uint16_t weather4Humidity;
    uint16_t weather5Temp;
    uint16_t weather5Humidity;
    uint16_t weather6Temp;
    uint16_t weather6Humidity;
    uint16_t weather7Temp;
    uint16_t weather7Humidity;
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
uint16_t EEMEM weather1TempEEPROM;
uint16_t EEMEM weather1HumidityEEPROM;
uint16_t EEMEM weather2TempEEPROM;
uint16_t EEMEM weather2HumidityEEPROM;
uint16_t EEMEM weather3TempEEPROM;
uint16_t EEMEM weather3HumidityEEPROM;
uint16_t EEMEM weather4TempEEPROM;
uint16_t EEMEM weather4HumidityEEPROM;
uint16_t EEMEM weather5TempEEPROM;
uint16_t EEMEM weather5HumidityEEPROM;
uint16_t EEMEM weather6TempEEPROM;
uint16_t EEMEM weather6HumidityEEPROM;
uint16_t EEMEM weather7TempEEPROM;
uint16_t EEMEM weather7HumidityEEPROM;
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

// Function prototypes
void uart_init(unsigned int baud);
void uart_send_char(char data);
void uart_send_string(const char *str);
char uart_receive(void);
void parse_receive_data(char *received_data);
void save_data_to_EEPROM();
void load_data_from_EEPROM();
void send_EEPROM_data();
void send_data_to_gui();
void send_eeprom_data_to_gui();
int main(void);

#endif  // SERIAL_H