// tested and worked at 2017/09/28 22:06

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "TWI/class_TWI.hpp"
#include "TWI/class_lcd_1602.hpp"
#include "MCU/class_rtc_3w.hpp"
#include "MCU/class_pin_io.hpp"


int main(){

	cTWI twiIOexpander(0x4E);
	cLCD1602 lcdTWI(&twiIOexpander);

	// define RTC control pins
	cIOPin rtcCE(&PORTD, 2, cIOPin::output);
	cIOPin rtcIO(&PORTD, 3, cIOPin::output);
	cIOPin rtcSCLK(&PORTD, 4, cIOPin::output);

	cDeviceRTC ds1302(&rtcCE, &rtcIO, &rtcSCLK);
	ds1302.set_RTC(2017, 25, 11, 14, 13, 0);


	lcdTWI.init();

	lcdTWI.clear();
	lcdTWI.write_String("testing RTC");
	_delay_ms(3000);


	lcdTWI.set_Cursor(0, 1);
	lcdTWI.write_String("hey pat");
	_delay_ms(2000);
	lcdTWI.set_Cursor(0, 2);
	lcdTWI.write_String("ich bin es");
	_delay_ms(2000);
	lcdTWI.clear();
	lcdTWI.write_String_XY(0, 1, "write StringXY()");
	lcdTWI.write_String_XY(0, 2, "work too");
	lcdTWI.no_Cursor();
	_delay_ms(3000);
	lcdTWI.cursor();
	lcdTWI.no_Display();
	_delay_ms(2000);
	lcdTWI.clear();
	lcdTWI.write_String_XY(0, 1, "Display was off");
	lcdTWI.display();
	_delay_ms(2000);
	lcdTWI.write_String_XY(0, 2, "blink off");
	lcdTWI.no_Blink();
	_delay_ms(2000);
	lcdTWI.home();
	lcdTWI.write_String_XY(0, 2, "blink on ");
	lcdTWI.blink();
	_delay_ms(2000);
	lcdTWI.clear();
	lcdTWI.write_Int_XY(6, 2, 1300);

	for(;;)
	{
		// display date
		ds1302.update_rtcTime();
		lcdTWI.clear();
		lcdTWI.write_String_XY(2, 1, "current date");
		lcdTWI.set_Cursor(3, 2);
		lcdTWI.write_Int(ds1302.rtcTime.year);
		lcdTWI.write_String("/");
		if(ds1302.rtcTime.month < 10)
		{
			 lcdTWI.write_String("0");
			 lcdTWI.write_Int(ds1302.rtcTime.month);
		}
		else
		{
			lcdTWI.write_Int(ds1302.rtcTime.month);
		}

		lcdTWI.write_String("/");
		if(ds1302.rtcTime.date < 10)
		{
			 lcdTWI.write_String("0");
			 lcdTWI.write_Int(ds1302.rtcTime.date);
		}
		else
		{
			lcdTWI.write_Int(ds1302.rtcTime.date);
		}

		_delay_ms(2000);

		// display time

		ds1302.update_rtcTime();
		lcdTWI.clear();
		lcdTWI.write_String_XY(2, 1, "current time");
		lcdTWI.set_Cursor(4, 2);
		if(ds1302.rtcTime.hours < 10){
			lcdTWI.write_String("0");
			lcdTWI.write_Int(ds1302.rtcTime.hours);
			lcdTWI.write_String(":");
			}
		else
			{
			lcdTWI.write_Int(ds1302.rtcTime.hours);
			lcdTWI.write_String(":");
			}
		if(ds1302.rtcTime.minutes < 10){
			lcdTWI.write_String("0");
			lcdTWI.write_Int(ds1302.rtcTime.minutes);
			lcdTWI.write_String(":");
			}
		else
			{
			lcdTWI.write_Int(ds1302.rtcTime.minutes);
			lcdTWI.write_String(":");
			}
		if(ds1302.rtcTime.seconds < 10){
			lcdTWI.write_String("0");
			lcdTWI.write_Int(ds1302.rtcTime.seconds);
			}
		else
			{
			lcdTWI.write_Int(ds1302.rtcTime.seconds);
			}
		_delay_ms(2000);
	}
}
