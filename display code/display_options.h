/*
 * display_options.h
 *
 * Created: 16/09/2024 9:43:03 PM
 *  Author: 61458
 */ 

#ifndef DISPLAY_OPTIONS_H_
#define DISPLAY_OPTIONS_H_

unsigned char** forecast;
unsigned char** humidity;
unsigned int temp_unit;
unsigned char* temp_reading();
void update_time_24hr(unsigned char* time);
unsigned int status_optical;
unsigned char* optical_message(void);
unsigned char* alarm_1;
unsigned char* alarm_2;
int alarm_detection(unsigned char* time);

#endif /* DISPLAY_OPTIONS_H_ */