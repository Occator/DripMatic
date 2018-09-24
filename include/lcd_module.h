#ifndef LCD_MODULE_H
#define LCD_MODULE_H

#include "twi_module.h"
#include <stdlib.h>

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_FUNCTIONSET 0x20
#define LCD_DISPLAYCONTROL 0x08

// flags for entry mode
#define LCD_ENTRYDECREASE 0x00
#define LCD_ENTRYINCREASE 0x02
#define LCD_ENTRYNOSHIFT 0x00
#define LCD_ENTRYSHIFT 0x01

// flags for display on/off
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for function set
#define LCD_INIT ( (0x00 | ENABLE) & ~RS ) & (~RW) // Used to set all the O/Ps on the PCF8574 to initialise the LCD
#define LCD_INTF8BITS 0x10  // Sub Mode of FUNCTION_SET : Select 8 bit interface (DL)
#define LCD_INTF4BITS 0x00  // Sub Mode of FUNCTION_SET : Select 4 bit interface (DL)
#define LCD_1LINE 0x00
#define LCD_2LINE 0x08
#define LCD_5x7DOTS 0x00
#define LCD_5x10DOTS 0x04

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define LCD_8BITMODE 0x38
#define LCD_4BITMODE 0x28

#define RS 0x01  //00000001
#define RW 0x02  //00000010
#define ENABLE 0x04  // 00000100
#define LCD_SET_DDADR 0x80

#define LCD_DDADR_LINE1 0x00
#define LCD_DDADR_LINE2 0x40
#define LCD_DDADR_LINE3 0x14
#define LCD_DDADR_LINE4 0x54


class cLCD{
public:

	cLCD(cTWI *twiDevice);
	~cLCD();

	void Clear();
	void Home();
	void Display();
	void NoDisplay();
	void Cursor();
	void NoCursor();
	void Blink();
	void NoBlink();
	void Backlight();
	void NoBacklight();
	void Write(uint8_t data);
	void WriteString(const char * string);
	void WriteInt(int data);
	void SetCursor(uint8_t x, uint8_t y);
	void WriteStringXY(uint8_t x, uint8_t y, const char * string);
	void WriteIntXY(uint8_t x, uint8_t y, uint16_t data);

private:
	void _Init();
	void _InitPriv();
	void _Begin();
	void _Command(uint8_t value);
	void _Send(uint8_t, uint8_t);
	void _Write4Bits(uint8_t value);
	void _PulseEnable(uint8_t data);

	cTWI *_twiLCD;
	uint8_t _displaycontrol = 0;
	uint8_t _displaymode = 0;
	uint8_t _lcdDevAddr = 0;

};

#endif
