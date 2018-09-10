#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "twi_module.h"
#include "lcd_module.h"
#include "adc_pin.h"
#include "rtc_3w.h"
#include "sdcard_module.h"
#include "uart_module.h"
#include "diskio.h"
#include "ff.h"


int main(){

	void displayDate(cLCD *lcd2004, cDeviceRTC *clock);
	void displayTime(cLCD *lcd2004, cDeviceRTC *clock);
	bool sensorReading = true;

	char convertBuffer[5];

	TCHAR header0[] = "no.";
	TCHAR header1[] = "DATE";
	TCHAR header2[] = "TIME";
	TCHAR header3[] = "Tensio-Val";

	FATFS fatFS;
	FIL systemLog;
	FIL records;
	FRESULT res_mount, res_open;

	cTWI ioExpander(0x4E);

	cIOPin redLED(&PORTD, 5, cIOPin::output);
	cIOPin yellowLED(&PORTD, 6, cIOPin::output);
	cIOPin greenLED(&PORTD, 7, cIOPin::output);

	cIOPin rtcCE(&PORTD, 2, cIOPin::output);
	cIOPin rtcIO(&PORTD, 3, cIOPin::output);
	cIOPin rtcSCLK(&PORTD, 4, cIOPin::output);

	cDeviceRTC ds1302(&rtcCE, &rtcIO, &rtcSCLK);
	ds1302.set_RTC(2018, 9, 10, 16, 15, 0);

	cADCPin tensiometer(0);

	cLCD userDisplay(&ioExpander);

	res_mount = f_mount(&fatFS, "", 0);
	if(res_mount == FR_OK)
	{
		res_open = f_open(&systemLog, "systemLog.txt", FA_WRITE | FA_OPEN_ALWAYS | FA_READ);
		if(res_open == FR_OK)
		{
			f_printf(&systemLog, "System start: ");
			ds1302.update_rtcTime();
			itoa(ds1302.rtcTime.year, convertBuffer, 10);
			f_puts(convertBuffer, &systemLog);
			f_putc('/', &systemLog);
			itoa(ds1302.rtcTime.month, convertBuffer, 10);
			f_puts(convertBuffer, &systemLog);
			f_putc('/', &systemLog);
			itoa(ds1302.rtcTime.date, convertBuffer, 10);
			f_puts(convertBuffer, &systemLog);
			f_putc('/', &systemLog);
			f_printf(&systemLog, " ");
			itoa(ds1302.rtcTime.hours, convertBuffer, 10);
			f_puts(convertBuffer, &systemLog);
			f_putc('/', &systemLog);
			itoa(ds1302.rtcTime.minutes, convertBuffer, 10);
			f_puts(convertBuffer, &systemLog);
			f_putc('/', &systemLog);
			itoa(ds1302.rtcTime.seconds, convertBuffer, 10);
			f_puts(convertBuffer, &systemLog);
			f_putc('\r', &systemLog);
			f_putc('\n', &systemLog);
		}
		f_close(&systemLog);
	}

	userDisplay.set_Cursor(5,1);
	userDisplay.write_String("DripMatic");
	userDisplay.set_Cursor(7, 2);
	userDisplay.write_String("ver.0.1");
	_delay_ms(3000);
	userDisplay.clear();

	uint16_t currentValue { 0 };
	uint16_t lastValue { 0 };

	for(;;)
	{
		greenLED.set_Pin(1);
		ds1302.update_rtcTime();

		displayDate(&userDisplay, &ds1302);
		displayTime(&userDisplay, &ds1302);
	}
}

void displayDate(cLCD *lcd, cDeviceRTC *clock)
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

void displayTime(cLCD *lcd, cDeviceRTC *clock)
{
	lcd->set_Cursor(15, 0);
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
	}
	else
	{
		lcd->write_Int(clock->rtcTime.minutes);
	}
}
