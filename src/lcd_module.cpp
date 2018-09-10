#include <util/delay.h>
#include "lcd_module.h"

cLCD::cLCD(cTWI *twiDevice) : _twiLCD(twiDevice)
{
	init();
}
cLCD::~cLCD()
{

}

void cLCD::init()
{
	_init_Priv();
}

void cLCD::_init_Priv()
{
	begin();
}

void cLCD::begin()
{

	_delay_ms(2000);
	// pull both RS and R/W low to begin commands
	_twiLCD->transmit(LCD_INIT);
	_delay_ms(10);

	// sequence to put LCD into 4-bit mode, this is according to the Hitachi HD44780 datasheet page 46, we start in 8-bit mode
	// we start in 8-bit mode
	_write4Bits(LCD_8BITMODE);
	_delay_ms(45);

	// second try
	_write4Bits(LCD_8BITMODE);
	_delay_ms(45);

	// third go!
	_write4Bits(LCD_8BITMODE);
	_delay_ms(50);

	//finally, set to 4-bit interface
	_write4Bits(LCD_4BITMODE);
	_delay_ms(15);

	command(LCD_FUNCTIONSET | LCD_INTF4BITS | LCD_2LINE | LCD_5x7DOTS);
	_delay_us(40);
	command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);  // display on, cursor on, blinking off
	_delay_us(40);
	command(LCD_ENTRYMODESET | LCD_ENTRYINCREASE | LCD_ENTRYNOSHIFT);
	_delay_us(40);
	command(LCD_CLEARDISPLAY);
	_delay_ms(64);
}

void cLCD::clear()
{
	command(LCD_CLEARDISPLAY);
	_delay_ms(64);
}


void cLCD::home()
{
	command(LCD_RETURNHOME);
	_delay_ms(64);
}

void cLCD::display()
{

	command(LCD_DISPLAYCONTROL | LCD_DISPLAYON| LCD_BLINKON | LCD_CURSORON);
	command(LCD_DISPLAYCONTROL | LCD_DISPLAYON);
	_delay_ms(20);
}

void cLCD::no_Display()
{
	command(LCD_DISPLAYCONTROL | LCD_DISPLAYOFF);
	_delay_ms(20);
}

void cLCD::cursor()
{
	command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_BLINKON | LCD_CURSORON);
	_delay_ms(200);
	command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSORON);
	_delay_ms(20);
}

void cLCD::no_Cursor()
{
	command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_BLINKON | LCD_CURSOROFF);
	_delay_ms(200);
	command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF);
	_delay_ms(20);
}

void cLCD::blink()
{
	command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_BLINKON | LCD_CURSOROFF);
	_delay_ms(200);
	command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_BLINKON);
	_delay_ms(20);
}

void cLCD::no_Blink()
{
	command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_BLINKOFF | LCD_CURSORON);
	_delay_ms(200);
	_twiLCD->transmit(LCD_BACKLIGHT);
	command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_BLINKOFF);
	_delay_ms(20);
}

void cLCD::backlight()
{
	_twiLCD->transmit(LCD_BACKLIGHT);
}

void cLCD::no_Backlight()
{
	_twiLCD->transmit(LCD_NOBACKLIGHT);
}
void cLCD::command(uint8_t value)
{
	_send(value, 0);
	_delay_ms(5);
}

void cLCD::_send(uint8_t value, uint8_t mode)
{
	uint8_t highnib = (value & 0xF0);
	uint8_t lownib = ((value << 4) & 0xF0);
	_write4Bits((highnib | mode));
	_write4Bits((lownib | mode));
}

void cLCD::_write4Bits(uint8_t value)
{
	_twiLCD->transmit(value & ~RW);
	_pulseEnable(value);
}

void cLCD::_pulseEnable(uint8_t data)
{
	_twiLCD->transmit(data | ENABLE);
    _delay_ms(5);
	_twiLCD->transmit(data & ~ENABLE);
	_delay_ms(1);
}

void cLCD::write(uint8_t data)
{
	_send(data, RS);
}


void cLCD::write_String(const char * string)
{
		while(*string != '\0'){
			write(*string);
			string++;
		}
}

void cLCD::set_Cursor(uint8_t x, uint8_t y )
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

    command(data);
}

void cLCD::write_String_XY(uint8_t x, uint8_t y, const char * string)
{
    set_Cursor(x, y);
    write_String(string);
}

void cLCD::write_Int_XY(uint8_t x, uint8_t y, uint16_t data)
{
	set_Cursor(x,y);
	char buffer[5];
	itoa(data, buffer, 10);
	write_String(buffer);
}

void cLCD::write_Int(int data)
{
	char buffer[5];
	itoa(data, buffer, 10);
	write_String(buffer);
}
