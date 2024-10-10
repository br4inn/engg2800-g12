/*
 * display_commands.c
 *
 * Created: 11/09/2024 2:18:21 PM
 *  Author: 61458
 */ 
#define F_CPU 8000000UL
#include "display_commands.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define BUSY   PIND6
#define RST    PIND7
#define DC     PINB1
#define CS     PINB0
#define SDA    PINB3
#define MISO   PINB4
#define SCL    PINB5


#define uchar  unsigned char
#define uint  unsigned int

#define Source_Pixel  128
#define Gate_Pixel    250

//***************************************************************//
//LUT_Flag Calling Waveform Markers
//Border_Flag  Border Markers,Only the first screen after initialization will refresh the border£¬Do not brush at other times
//***************************************************************//
unsigned long LUT_Flag=0,Border_Flag=0;

//Full refresh waveform, refresh time 800ms
const PROGMEM unsigned char lut_R20_GC[] =
{
	0x01,	0x00,	0x14,	0x14,	0x01,	0x01,	0x00,	0x01,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
};
const PROGMEM unsigned char lut_R21_GC[] =
{
	0x01,	0x60,	0x14,	0x14,	0x01,	0x01,	0x00,	0x01,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
};
const PROGMEM unsigned char  lut_R22_GC[] =
{
	0x01,	0x60,	0x14,	0x14,	0x01,	0x01,	0x00,	0x01,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
};
const PROGMEM unsigned char lut_R23_GC[] =
{
	0x01,	0x90,	0x14,	0x14,	0x01,	0x01,	0x00,	0x01,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
};
const PROGMEM unsigned char lut_R24_GC[] =
{
	0x01,	0x90,	0x14,	0x14,	0x01,	0x01,	0x00,	0x01,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
};

// Partially refresh waveform, screen refresh time 400ms
const static unsigned char lut_R20_DU[] PROGMEM =
{
	0x01,	0x00,	0x14,	0x01,	0x01,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
};
const static unsigned char lut_R21_DU[] PROGMEM =
{
	0x01,	0x20,	0x14,	0x01,	0x01,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
};
const static unsigned char lut_R22_DU[] PROGMEM =
{
	0x01,	0x80,	0x14,	0x01,	0x01,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
};
const static unsigned char lut_R23_DU[] PROGMEM =
{
	0x01,	0x40,	0x14,	0x01,	0x01,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
};
const static unsigned char lut_R24_DU[] PROGMEM =
{
	0x01,	0x00,	0x14,	0x01,	0x01,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
};


// WRITE COMMAND  xxxxxxxxxxxxxxxxxxx
void EPD_W21_WriteCMD(unsigned char INIT_COM)
{
	
	//SPCR = ((1 << SPE) | (1 << MSTR));
	
	PORTB &= ~(1 << CS);
	PORTB &= ~(1 << DC);
	
	SPDR = INIT_COM;
	
	while(!(SPSR & (1 << SPIF)));
	
	PORTB |= (1 << CS);
	PORTB |= (1 << DC);
	
}

// WRITE DATA  xxxxxxxxxxxxxxxxxxxxx
void EPD_W21_WriteDATA(unsigned char INIT_DATA)
{

	//SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
	
	PORTB &= ~(1 << CS);
	
	SPDR = INIT_DATA;
	
	while(!(SPSR & (1 << SPIF)));
	
	PORTB |= (1 << CS);
	
}


//*******************EPD_init(void)***************************//
//Initialize settings. You must re-initialize settings after resetting.
//******************************************************//
void EPD_init(void)
{
	
	LUT_Flag=0,Border_Flag=0; //(test with and without this line)

	EPD_W21_WriteCMD(0x00);
	EPD_W21_WriteDATA(0xFB);//You can change the scanning direction, FF -> FB F3 F7 to rotate and flip left and right and up and down

	
	EPD_W21_WriteCMD(0x01);
	EPD_W21_WriteDATA(0x03);
	EPD_W21_WriteDATA(0x00);
	EPD_W21_WriteDATA(0x3F);
	EPD_W21_WriteDATA(0x3F);
	EPD_W21_WriteDATA(0x03);

	EPD_W21_WriteCMD(0x03);
	EPD_W21_WriteDATA(0x00);

	EPD_W21_WriteCMD(0x06);
	EPD_W21_WriteDATA(0x27);
	EPD_W21_WriteDATA(0x27);
	EPD_W21_WriteDATA(0x2F);

	EPD_W21_WriteCMD(0x30);
	EPD_W21_WriteDATA(0x09);

	EPD_W21_WriteCMD(0x60);
	EPD_W21_WriteDATA (0x22);

	EPD_W21_WriteCMD(0x82);        // vcom
	EPD_W21_WriteDATA(0x00);      //

	EPD_W21_WriteCMD(0xE3);
	EPD_W21_WriteDATA(0x00);

	EPD_W21_WriteCMD(0x41);
	EPD_W21_WriteDATA(0x00);
	
	EPD_W21_WriteCMD(0x61);
	EPD_W21_WriteDATA(0x80);
	EPD_W21_WriteDATA(0x00);
	EPD_W21_WriteDATA(0xFA);
	
	EPD_W21_WriteCMD(0x65);
	EPD_W21_WriteDATA(0x00);
	EPD_W21_WriteDATA(0x00);
	EPD_W21_WriteDATA(0x00);

	EPD_W21_WriteCMD(0X50);
	EPD_W21_WriteDATA(0xB7);
}


//****************EPD_refresh(void)************************//
//Screen refresh command
//******************************************************//
void EPD_refresh(void)
{
	Border_Flag=1;
	EPD_W21_WriteCMD(0x17);
	EPD_W21_WriteDATA(0xA5);
	lcd_chkstatus();
}


//***************EPD_sleep(void)************************//
//Enter sleep
//After exiting sleep, you need to re-initialize the settings. Use GC to refresh the first screen, and then use DU to refresh the screen.
//******************************************************//
void EPD_sleep(void)
{
	EPD_W21_WriteCMD(0X07); //deep sleep
	EPD_W21_WriteDATA(0xA5);
}



//***************lut_GC(void)************************//
//Call GC waveform
//***************************************************//
void lut_GC(void)
{
	unsigned int count;
	unsigned char displayChar;
	
	EPD_W21_WriteCMD(0x20);							//vcom
	for(count=0;count<56;count++){
		displayChar = pgm_read_byte(&lut_R20_GC[count]);
	EPD_W21_WriteDATA(displayChar);}
	
	EPD_W21_WriteCMD(0x21);							//red not use
	for(count=0;count<56;count++){
		displayChar = pgm_read_byte(&lut_R21_GC[count]);
		EPD_W21_WriteDATA(displayChar);
	}

	EPD_W21_WriteCMD(0x24);							//bb b
	for(count=0;count<56;count++){
		displayChar = pgm_read_byte(&lut_R24_GC[count]);
		EPD_W21_WriteDATA(displayChar);
	}
	
	if (LUT_Flag==0)
	{

		EPD_W21_WriteCMD(0x22);							//bw r
		for(count=0;count<56;count++){
			displayChar = pgm_read_byte(&lut_R22_GC[count]);
		EPD_W21_WriteDATA(displayChar);}

		EPD_W21_WriteCMD(0x23);							//wb w
		for(count=0;count<56;count++){
			displayChar = pgm_read_byte(&lut_R23_GC[count]);
		EPD_W21_WriteDATA(displayChar);}
		LUT_Flag=1;
	}
	else
	{
		
		EPD_W21_WriteCMD(0x22);							//bw r
		for(count=0;count<56;count++){
			displayChar = pgm_read_byte(&lut_R23_GC[count]);
		EPD_W21_WriteDATA(displayChar);}

		EPD_W21_WriteCMD(0x23);							//wb w
		for(count=0;count<56;count++){
			displayChar = pgm_read_byte(&lut_R22_GC[count]);
		EPD_W21_WriteDATA(displayChar);}
		LUT_Flag=0;
	}
	
}


//***************lut_DU(void)************************//
//Call DU waveform
//***************************************************//
void lut_DU(void)
{
	unsigned int count;
	unsigned char displayChar;
	
	EPD_W21_WriteCMD(0x20);							//vcom
	for(count=0;count<56;count++){
		displayChar = pgm_read_byte(&lut_R20_DU[count]);
	EPD_W21_WriteDATA(displayChar);}
	
	EPD_W21_WriteCMD(0x21);							//red not use
	for(count=0;count<56;count++){
		displayChar = pgm_read_byte(&lut_R21_DU[count]);
	EPD_W21_WriteDATA(displayChar);}

	EPD_W21_WriteCMD(0x24);							//bb b
	for(count=0;count<56;count++){
		displayChar = pgm_read_byte(&lut_R24_DU[count]);
	EPD_W21_WriteDATA(displayChar);}
	
	if (LUT_Flag==0)
	{
		
		EPD_W21_WriteCMD(0x22);							//bw r
		for(count=0;count<56;count++){
			displayChar = pgm_read_byte(&lut_R22_DU[count]);
		EPD_W21_WriteDATA(displayChar);}

		EPD_W21_WriteCMD(0x23);							//wb w
		for(count=0;count<56;count++){
			displayChar = pgm_read_byte(&lut_R23_DU[count]);
		EPD_W21_WriteDATA(displayChar);}
		LUT_Flag=1;
	}
	else
	{
		
		EPD_W21_WriteCMD(0x22);							//bw r
		for(count=0;count<56;count++){
			displayChar = pgm_read_byte(&lut_R23_DU[count]);
		EPD_W21_WriteDATA(displayChar);}

		EPD_W21_WriteCMD(0x23);							//wb w
		for(count=0;count<56;count++){
			displayChar = pgm_read_byte(&lut_R22_DU[count]);
		EPD_W21_WriteDATA(displayChar);}
		LUT_Flag=0;
	}
	
}


//***************PIC_display_DU**********************//
//Call DU waveform to refresh the screen
//***************************************************//
void PIC_display_DU(const unsigned char* picData)
{
	unsigned int i;
	unsigned char displayChar;
	
	EPD_W21_WriteCMD(0X50);   //
	EPD_W21_WriteDATA(0xD7);  // Border
	
	EPD_W21_WriteCMD(0x13);	  //Transfer new data
	for(i=0;i<(Gate_Pixel*Source_Pixel/8);i++)
	{
		displayChar = pgm_read_byte(picData);
		EPD_W21_WriteDATA(displayChar);
		picData++;
	}
	
	lut_DU();
	EPD_refresh();
}

//***************Clean display_GU**********************//
//Call GC waveform to refresh the screen
//***************************************************//
void Clean_display_GC(const unsigned char Dat)
{
	unsigned int i;
	
	//***********************************************************************//
	//Border_Flag   Only the first screen after initialization refreshes the border, and no refreshes at other times

	if (Border_Flag==1)
	{
		EPD_W21_WriteCMD(0X50);      //
		EPD_W21_WriteDATA(0xD7);     // no refreshes Border
	}
	//***********************************************************************//

	EPD_W21_WriteCMD(0x13);
	for(i=0;i<(Gate_Pixel*Source_Pixel/8);i++)
	{
		EPD_W21_WriteDATA(Dat);
		
	}

	lut_GC();
	EPD_refresh();
}


//***************PIC_display_GU**********************//
//Call GC waveform to refresh the screen
//***************************************************//
void PIC_display_GC(const unsigned char* picData)
{
	unsigned int i;
	unsigned char displayChar;
	
	//***********************************************************************//
	////Border_Flag   Only the first screen after initialization refreshes the border, and no refreshes at other times

	if (Border_Flag==1)
	{
		EPD_W21_WriteCMD(0X50);      //
		EPD_W21_WriteDATA(0xD7);     // no refreshes Border
	}
	//***********************************************************************//

	EPD_W21_WriteCMD(0x13);
	for(i=0;i<(Gate_Pixel*Source_Pixel/8);i++)
	{
		displayChar = pgm_read_byte(picData);
		EPD_W21_WriteDATA(displayChar);
		picData++;
		
	}

	lut_GC();
	EPD_refresh();
}

//***************lcd_chkstatus***********************//
//READ BUSY Pin level = L means the screen driver chip is busy and cannot send data or instructions.
//Pin level = H means the screen driver chip is in idle state and can send data and instructions
//***************************************************//
void lcd_chkstatus(void)
{
	while(1)
	{
		if(PIND & (1 << BUSY))
		break;
	}
}


void EPD_Reset(void)
{
	PORTD |= (1 << RST);
	_delay_ms(20);	//At least 10ms delay
	PORTD &= ~(1 << RST);		    // Module reset
	_delay_ms(20);//At least 10ms delay
	PORTD |= (1 << RST);
	_delay_ms(20);//At least 10ms delay
}



	