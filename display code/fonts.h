/*
 * fonts.h
 *
 * Created: 16/09/2024 12:57:04 PM
 *  Author: 61458
 */ 


#ifndef FONTS_5X8_H_
#define FONTS_5X8_H_

void EPD_Part_Char(unsigned int x_start, unsigned int y_start, const unsigned char* char_bitmap , unsigned int PART_COLUMN, unsigned int PART_LINE);
void print_char_large(unsigned int x_start, unsigned int y_start, char character, unsigned int PART_COLUMN, unsigned int PART_LINE)
void print_char(unsigned int x_start, unsigned int y_start, char character , unsigned int PART_COLUMN, unsigned int PART_LINE)
void EPD_Part_Str(unsigned int x_start,unsigned int y_start,char* string ,unsigned int PART_COLUMN,unsigned int PART_LINE, unsigned int large_size);
unsigned int status_pc;
const unsigned char pc_icon(void);


#endif /* FONTS_5X8_H_ */