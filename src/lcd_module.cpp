#include <util/delay.h>
#include "lcd_module.h"

cLCD::cLCD(cTWI *twiDevice) : _twiLCD(twiDevice)
{
	_Init();
}
cLCD::~cLCD()
{}

void cLCD::Clear()
{
	_Command(LCD_CLEARDISPLAY);
	_delay_ms(64);
}

void cLCD::Home()
{
	_Command(LCD_RETURNHOME);
	_delay_ms(64);
}

void cLCD::Display()
{

	_Command(LCD_DISPLAYCONTROL | LCD_DISPLAYON| LCD_BLINKON | LCD_CURSORON);
	_Command(LCD_DISPLAYCONTROL | LCD_DISPLAYON);
	_delay_ms(20);
}

void cLCD::NoDisplay()
{
	_Command(LCD_DISPLAYCONTROL | LCD_DISPLAYOFF);
	_delay_ms(20);
}

void cLCD::Cursor()
{
	_Command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_BLINKON | LCD_CURSORON);
	_delay_ms(200);
	_Command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSORON);
	_delay_ms(20);
}

void cLCD::NoCursor()
{
	_Command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_BLINKON | LCD_CURSOROFF);
	_delay_ms(200);
	_Command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF);
	_delay_ms(20);
}

void cLCD::Blink()
{
	_Command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_BLINKON | LCD_CURSOROFF);
	_delay_ms(200);
	_Command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_BLINKON);
	_delay_ms(20);
}

void cLCD::NoBlink()
{
	_Command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_BLINKOFF | LCD_CURSORON);
	_delay_ms(200);
	_twiLCD->transmit(LCD_BACKLIGHT);
	_Command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_BLINKOFF);
	_delay_ms(20);
}

void cLCD::Backlight()
{
	_twiLCD->transmit(LCD_BACKLIGHT);
}

void cLCD::NoBacklight()
{
	_twiLCD->transmit(LCD_NOBACKLIGHT);
}

void cLCD::Write(uint8_t data)
{
	_Send(data, RS);
}

void cLCD::WriteString(const char * string)
{
		while(*string != '\0'){
			Write(*string);
			string++;
		}
}

void cLCD::WriteInt(int data)
{
	char buffer[5];
	itoa(data, buffer, 10);
	WriteString(buffer);
}

void cLCD::SetCursor(uint8_t x, uint8_t y )
{
    uint8_t data = 0;

    switch (y)
    {
        case 0:    // 1. Zeile
            data = LCD_SET_DDADR + LCD_DDADR_LINE1 + x;
            break;

        case 1:    // 2. Zeile
            data = LCD_SET_DDADR + LCD_DDADR_LINE2 + x;
            break;

        case 2:    // 3. Zeile
            data = LCD_SET_DDADR + LCD_DDADR_LINE3 + x;
            break;

        case 3:    // 4. Zeile
            data = LCD_SET_DDADR + LCD_DDADR_LINE4 + x;
            break;

        default:
            return;  // für den Fall einer falschen Zeile
    }

    _Command(data);
}

void cLCD::WriteStringXY(uint8_t x, uint8_t y, const char * string)
{
    SetCursor(x, y);
    WriteString(string);
}

void cLCD::WriteIntXY(uint8_t x, uint8_t y, uint16_t data)
{
	SetCursor(x,y);
	char buffer[5];
	itoa(data, buffer, 10);
	WriteString(buffer);
}

void cLCD::_Init()
{
	_InitPriv();
}

void cLCD::_InitPriv()
{
	_Begin();
}

void cLCD::_Begin()
{

	_delay_ms(2000);
	// pull both RS and R/W low to begin commands
	_twiLCD->transmit(LCD_INIT);
	_delay_ms(10);

	// sequence to put LCD into 4-bit mode, this is according to the Hitachi HD44780 datasheet page 46, we start in 8-bit mode
	// we start in 8-bit mode
	_Write4Bits(LCD_8BITMODE);
	_delay_ms(45);

	// second try
	_Write4Bits(LCD_8BITMODE);
	_delay_ms(45);

	// third go!
	_Write4Bits(LCD_8BITMODE);
	_delay_ms(50);

	//finally, set to 4-bit interface
	_Write4Bits(LCD_4BITMODE);
	_delay_ms(15);

	_Command(LCD_FUNCTIONSET | LCD_INTF4BITS | LCD_2LINE | LCD_5x7DOTS);
	_delay_us(40);
	_Command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);  // display on, cursor on, blinking off
	_delay_us(40);
	_Command(LCD_ENTRYMODESET | LCD_ENTRYINCREASE | LCD_ENTRYNOSHIFT);
	_delay_us(40);
	_Command(LCD_CLEARDISPLAY);
	_delay_ms(64);
}

void cLCD::_Command(uint8_t value)
{
	_Send(value, 0);
	_delay_ms(5);
}

void cLCD::_Send(uint8_t value, uint8_t mode)
{
	uint8_t highnib = (value & 0xF0);
	uint8_t lownib = ((value << 4) & 0xF0);
	_Write4Bits((highnib | mode));
	_Write4Bits((lownib | mode));
}

void cLCD::_Write4Bits(uint8_t value)
{
	_twiLCD->transmit(value & ~RW);
	_PulseEnable(value);
}

void cLCD::_PulseEnable(uint8_t data)
{
	_twiLCD->transmit(data | ENABLE);
	_delay_ms(5);
	_twiLCD->transmit(data & ~ENABLE);
	_delay_ms(1);
}
