#include <util/delay.h>
#include "class_lcd_1602.hpp"

cLCD1602::cLCD1602(cTWI *twiDevice) : _twiLCD(twiDevice)
{

}
cLCD1602::~cLCD1602()
{

}

void cLCD1602::init()
{
	_init_Priv();
}

void cLCD1602::_init_Priv()
{
	begin();
}

void cLCD1602::begin()
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
	command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKON);  // display on, cursor on, blinking off
	_delay_us(40);
	command(LCD_ENTRYMODESET | LCD_ENTRYINCREASE | LCD_ENTRYNOSHIFT);
	_delay_us(40);
	command(LCD_CLEARDISPLAY);
	_delay_ms(64);
	//clear();
}

void cLCD1602::clear()
{	
	set_Cursor(0, 1);
	uint8_t i;
	for(i = 0; i < 50; i++)
	{	

		write(' ');
	}
	set_Cursor(0,1);
}


void cLCD1602::home()
{
	command(LCD_RETURNHOME);
	//_delay_ms(64);
}

void cLCD1602::display()
{
	command(LCD_DISPLAYCONTROL | LCD_DISPLAYON);
	_delay_ms(200);
}

void cLCD1602::no_Display()
{
	command(LCD_DISPLAYCONTROL | LCD_DISPLAYOFF);
	_delay_ms(200);
}

void cLCD1602::cursor()
{
	command(LCD_DISPLAYCONTROL | LCD_CURSORON);
	_delay_ms(200);
}

void cLCD1602::no_Cursor()
{
	
	command(LCD_DISPLAYCONTROL & ~LCD_CURSORON);
	_delay_ms(200);
}



void cLCD1602::blink()
{
	command(0x0F);
	_delay_ms(200);
}

void cLCD1602::no_Blink()
{
	command(0xE);
	_delay_ms(200);
	_twiLCD->transmit(LCD_BACKLIGHT);
}

void cLCD1602::backlight()
{
	_twiLCD->transmit(LCD_BACKLIGHT);
	_delay_ms(10);
}

void cLCD1602::no_Backlight()
{
	_twiLCD->transmit(LCD_NOBACKLIGHT);
//	_delay_ms(10);
}
void cLCD1602::command(uint8_t value)
{
	_send(value, 0);
	_delay_ms(50);
}

void cLCD1602::_send(uint8_t value, uint8_t mode)
{
	uint8_t highnib = (value & 0xF0);
	uint8_t lownib = ((value << 4) & 0xF0);
	_write4Bits((highnib | mode));
	_write4Bits((lownib | mode));
}

void cLCD1602::_write4Bits(uint8_t value)
{
	value |= LCD_BACKLIGHT;
	_twiLCD->transmit(value & ~RW);
	_pulseEnable(value);
}

void cLCD1602::_pulseEnable(uint8_t data)
{
	_twiLCD->transmit(data | ENABLE);
    _delay_ms(5);
	_twiLCD->transmit(data & ~ENABLE);
	_delay_ms(1);
}

void cLCD1602::write(uint8_t data)
{
	_send(data, RS);
}


void cLCD1602::write_String(const char * string){
	
		while(*string != '\0'){
			write(*string);
			string++;
		}
}

void cLCD1602::set_Cursor( uint8_t x, uint8_t y )
{
    uint8_t data = 0;
 
    switch (y)
    {
        case 1:    // 1. Zeile
            data = LCD_SET_DDADR + LCD_DDADR_LINE1 + x;
            break;
 
        case 2:    // 2. Zeile
            data = LCD_SET_DDADR + LCD_DDADR_LINE2 + x;
            break;
 
        case 3:    // 3. Zeile
            data = LCD_SET_DDADR + LCD_DDADR_LINE3 + x;
            break;
 
        case 4:    // 4. Zeile
            data = LCD_SET_DDADR + LCD_DDADR_LINE4 + x;
            break;
 
        default:
            return;                                   // für den Fall einer falschen Zeile
    }
 
    command(data);
}

void cLCD1602::write_String_XY(uint8_t x, uint8_t y, char * string) {
    set_Cursor(x, y);
    write_String(string);
}

void cLCD1602::write_Int_XY(uint8_t x, uint8_t y, int data){
	char buffer[5];
	itoa(data, buffer, 10);
	write_String(buffer);
}