
//This code should be put in main.c 

#define F_CPU 8000000UL
#define BAUD_RATE 200000

#include <util/delay.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <time.h>
#include <stdio.h>

#include "display_commands.h"
#include "fonts.h"
#include "display_options.h"

#define BUSY   PIND6
#define RST    PIND7
#define DC     PINB1
#define CS     PINB0
#define SDA    PINB3
#define MISO   PINB4
#define SCL    PINB5


#define uchar  unsigned char
#define uint  unsigned int

#define Source_Pixel  122
#define Gate_Pixel    250

char* set_time = "00:00";

//SCREENS FOR TESTING

//SCREEN 1
void screen_option_one(void){
	//Time and date
	EPD_Part_Str(Source_Pixel-5, 5, "00:00", 32, 32, 1);
	update_time_24hr(set_time);
	EPD_Part_Str(Source_Pixel-38, 5, "16/9/24", 32, 32, 1);
	
	//Weather
	EPD_Part_Str(Source_Pixel-75, 5, "forecast:", 8, 5, 0); 
	EPD_Part_Str(Source_Pixel-75, 50, forecast[0], 8, 5, 0); //button 1
	EPD_Part_Str(Source_Pixel-75, 85, "humidity:", 8, 5, 0); 
	EPD_Part_Str(Source_Pixel-75, 130, humidity[0], 8, 5, 0); //button 1
	EPD_Part_Str(Source_Pixel-75, 160, "temp:", 8, 5, 0);
	EPD_Part_Str(Source_Pixel-75, 190, temp_reading(), 8, 5, 0); //button 2
	
	//Alarms
	EPD_Part_Str(Source_Pixel-85, 5, "Alarms set:", 8, 5, 0);
	EPD_Part_Str(Source_Pixel-95, 5, alarm_1, 8, 5, 0);
	EPD_Part_Str(Source_Pixel-105, 5, alarm_2, 8, 5, 0);
	
	//PC communication
	EPD_Part_Str(Source_Pixel-85, 85, "PC Communication:", 8, 5, 0); //button 3
	EPD_Part_Char(Source_Pixel-95, 85, pc_icon(), 8, 8, 0);
	
	//Optical link
	EPD_Part_Str(Source_Pixel-110, 5, optical_message(), 8, 5, 0); //button 4
}

//SCREEN 2
void screen_option_two(void){ //button 5
	//Time and date
	EPD_Part_Str(Source_Pixel-5, 5, "00:00", 8, 5);
	update_time_24hr(set_time);
	EPD_Part_Str(Source_Pixel-20, 5, "16/9/24", 8, 5);
	
	//Alarms
	EPD_Part_Str(Source_Pixel-30, 5, "Alarm 1 has been triggered.", 8, 5);
	EPD_Part_Str(Source_Pixel-40, 5, "Message: Wake me up!", 8, 5);
	EPD_Part_Str(Source_Pixel-50, 5, "Press button 5 to disable", 8, 5);
}

int main(void)
{
    DDRB = 0xFF;
    PORTB = 0xFF;
	DDRB |= (1 << DC) | (1 << CS) | (1 << SDA) | (1 << SCL) | (1 << 2);
    DDRD |= (1 << RST);
    PORTB |= (1 << CS) | (1 << DC);
    PORTD |= (1 << RST);
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0) | (1 << SPR1);
    
    _delay_ms(2000);
	
    EPD_Reset();
    EPD_init(); //EPD init
    Clean_display_GC(0x00);
    _delay_ms(2000);
	
	screen_option_one();
	EPD_refresh();

	while (1) {
		update_time_24hr(set_time);
		EPD_Part_Str(Source_Pixel-32, 5, set_time, 32, 32, 1);
		EPD_refresh();
	}
}