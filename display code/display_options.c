/*
 * CFile3.c
 *
 * Created: 16/09/2024 8:04:05 PM
 *  Author: 61458
 */ 
#include <util/delay.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>

#include "display_commands.h"
#include  "fonts.h"
#include "display_options.h"
#include "timer0.h"

//FORECAST
unsigned char* forecast[5] = {"sunny", "cloudy", "rain", "snow", "storm"};

//HUMIDITY
unsigned char* humidity[5] = {"79", "80", "73", "43", "73"};

//TEMPERATURE
unsigned int temp_unit=0;
unsigned char* temp_reading(){
	unsigned char* messageTemp;
	if(temp_unit){
		messageTemp = "72F";
	}
	else{
		messageTemp = "22°C";
	}
	return messageTemp;
}
	
//TIME
//char* current_time = "00:00";
//Need to write function that gets the current time. Will most likely be in ms.
//uint32_t get_current_time(void){
	//return 0;
//}

//Function that updates time printed on display
void update_time_24hr(unsigned char* time){
	unsigned int time_seconds = get_current_time()/1000;
	if(!(time_seconds%60)){
		if(time == "23:59  "){
			time = "00:00  ";
		}
		else if(time[4]=='9'){
			if(time[3]=='6'){
				if(time[1]=='9'){
					time[0] = time[0]+1;
					time[1] = '0';
				}
				else{
					time[1] = time[1]+1;
				}
				time[3] = '0';
			}
			else{
				time[3] = time[1]+1;
			}
			time[4] = '0';
		}
		else{
			time[4] = time[4]+1;
		}
	}
}

//OPTICAL LINK
unsigned int status_optical=0;
unsigned char* optical_message(){
	unsigned char* messageOptical;
	if(status_optical){
		messageOptical = "optical link: yes";
		status_optical = 0;
	}
	else{
		messageOptical = "optical link: no";
		status_optical = 1;
	}
	return messageOptical;
}

//ALARMS
unsigned char* alarm_1 = "1. 00:01";
unsigned char* alarm_2 = "2. 12:30";

int alarm_detection(unsigned char* time){
	if(time==alarm_1 || time==alarm_2){
		return 1;
	}
	return 0;
}

