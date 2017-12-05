#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "class_TWI.hpp"
#include "class_lcd_1602.hpp"
#include "class_adc_pin.hpp"
#include "class_pin_io.hpp"
#include "class_rtc_3w.hpp"


int main(){

	cTWI twiIOexpander(0x4E);
	cLCD1602 lcdTWI(&twiIOexpander);


	cIOPin redLED(&PORTB, 4, cIOPin::output);
	redLED.set_Pin(1);
	cIOPin yellowLED(&PORTB, 3, cIOPin::output);
	redLED.set_Pin(1);
	cIOPin greenLED(&PORTB, 2, cIOPin::output);
	redLED.set_Pin(1);

	// define RTC control pins
	cIOPin rtcCE(&PORTD, 2, cIOPin::output);
	cIOPin rtcIO(&PORTD, 3, cIOPin::output);
	cIOPin rtcSCLK(&PORTD, 4, cIOPin::output);

	cDeviceRTC ds1302(&rtcCE, &rtcIO, &rtcSCLK);
	ds1302.set_RTC(2017, 25, 11, 14, 13, 0);

	cADCPin LDR(0);
	LDR.init();
	// temp sensor
	cADCPin LM35(2);
	LM35.init();

	lcdTWI.init();

	lcdTWI.clear();
	lcdTWI.write_String("testing ADC");
	_delay_ms(3000);


	lcdTWI.set_Cursor(0, 1);
	lcdTWI.clear();
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

	int8_t count = 0;

	for(;;)
	{

		if(count > 4)	yellowLED.set_Pin(0);
		else yellowLED.set_Pin(1);
		lcdTWI.clear();
		lcdTWI.write_String_XY(4, 1, "ADC-Value");
		auto adcValue = LDR.read();
		lcdTWI.set_Cursor(7, 2);
		lcdTWI.write_Int(adcValue);
		_delay_ms(500);
		lcdTWI.write_String_XY(2, 1, "Temperature");
		auto tempValue = LM35.read();
		lcdTWI.set_Cursor(7, 2);
		lcdTWI.write_Int(tempValue);
		_delay_ms(500);
		redLED.set_Pin(0);
		yellowLED.set_Pin(0);
		_delay_ms(500);
		count++;
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
