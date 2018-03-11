#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "twi_module.h"
#include "lcd_module.h"
#include "adc_pin.h"
#include "pin_io.h"
#include "rtc_3w.h"
#include "spi_module.h"

void display_Date_Frame(cLCD1602 *lcd, cDeviceRTC *clock);
void display_Time_Frame(cLCD1602 *lcd, cDeviceRTC *clock);

int main(){
	cIOPin redLED(&PORTB, 2, cIOPin::output);
	cIOPin yellowLED(&PORTB, 3, cIOPin::output);
	cIOPin greenLED(&PORTB, 4, cIOPin::output);

	// define RTC control pins
	cIOPin rtcCE(&PORTD, 2, cIOPin::output);
	cIOPin rtcIO(&PORTD, 3, cIOPin::output);
	cIOPin rtcSCLK(&PORTD, 4, cIOPin::output);

	cTWI twiIOexpander(0x4E);

	cDeviceRTC ds1302(&rtcCE, &rtcIO, &rtcSCLK);
	ds1302.set_RTC(2017, 25, 11, 14, 14, 45);

	cADCPin Tensiometer(0);

	redLED.set_Pin(1);
	yellowLED.set_Pin(1);
	greenLED.set_Pin(1);

	cLCD1602 lcdTWI(&twiIOexpander);
	lcdTWI.init();

	lcdTWI.set_Cursor(3,1);
	lcdTWI.write_String("irrigation bot");
	lcdTWI.set_Cursor(7, 2);
	lcdTWI.write_String("ver.01");
	_delay_ms(3000);
	lcdTWI.clear();
	uint32_t sensorValue = 0;

	for(;;)
	{
		ds1302.update_rtcTime();

		display_Date_Frame(&lcdTWI, &ds1302);
		display_Time_Frame(&lcdTWI, &ds1302);

		uint32_t adcValue = 0;

		adcValue = Tensiometer.read();
		lcdTWI.write_String_XY(5, 2, "ADC-value");
		lcdTWI.set_Cursor(8, 3);
		lcdTWI.write_Int(adcValue - 90);
		lcdTWI.write_String("  ");
	}
}

void display_Date_Frame(cLCD1602 *lcd, cDeviceRTC *clock)
{
	lcd->set_Cursor(0, 0);
	// frame_date
	lcd->write_Int(clock->rtcTime.year);
	lcd->write_String("/");
	if(clock->rtcTime.month < 10)
	{
		 lcd->write_String("0");
		 lcd->write_Int(clock->rtcTime.month);
	}
	else
	{
		lcd->write_Int(clock->rtcTime.month);
	}
	lcd->write_String("/");
	if(clock->rtcTime.date < 10)
	{
		 lcd->write_String("0");
		 lcd->write_Int(clock->rtcTime.date);
	}
	else
	{
		lcd->write_Int(clock->rtcTime.date);
	}
}

void display_Time_Frame(cLCD1602 *lcd, cDeviceRTC *clock)
{
	lcd->set_Cursor(12, 0);
	if(clock->rtcTime.hours < 10)
	{
		lcd->write_String("0");
		lcd->write_Int(clock->rtcTime.hours);
		lcd->write_String(":");
	}
	else
	{
		lcd->write_Int(clock->rtcTime.hours);
		lcd->write_String(":");
	}
	if(clock->rtcTime.minutes < 10)
	{
		lcd->write_String("0");
		lcd->write_Int(clock->rtcTime.minutes);
		lcd->write_String(":");
	}
	else
	{
		lcd->write_Int(clock->rtcTime.minutes);
		lcd->write_String(":");
	}
	if(clock->rtcTime.seconds < 10)
	{
		lcd->write_String("0");
		lcd->write_Int(clock->rtcTime.seconds);
	}
	else
	{
		lcd->write_Int(clock->rtcTime.seconds);
	}
}
