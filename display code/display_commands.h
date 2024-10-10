/*
 * display_commands.h
 *
 * Created: 11/09/2024 2:17:10 PM
 *  Author: 61458
 */ 


#ifndef DISPLAY_COMMANDS_H_
#define DISPLAY_COMMANDS_H_

void lut_GC(void);//Write full brush waveform
void lut_DU(void);//Write partial brush waveform
void EPD_init(void);//Write Initialization
void PIC_display_GC(const unsigned char* picData);//full brush
void PIC_display_DU(const unsigned char* picData);//partial brush
void EPD_sleep(void);//sleep
void EPD_refresh(void);//Start refreshing the screen
void lcd_chkstatus(void);//Determine whether the screen is idle
void EPD_Reset(void);//Hardware Reset


#endif /* DISPLAY_COMMANDS_H_ */