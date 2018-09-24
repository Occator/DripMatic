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


	void DisplayDate(cLCD *lcd2004, cDeviceRTC *clock);
	void DisplayTime(cLCD *lcd2004, cDeviceRTC *clock);
	bool sensorReading = true;
	int16_t runningNumber { 0 };

	char convertBuffer[5];

	TCHAR header0[] = "no.";
	TCHAR header1[] = "DATE";
	TCHAR header2[] = "TIME";
	TCHAR header3[] = "Tensio-Val";

	cIOPin rtcCE(&PORTD, 2, cIOPin::output);
	cIOPin rtcIO(&PORTD, 3, cIOPin::output);
	cIOPin rtcSCLK(&PORTD, 4, cIOPin::output);

	FATFS fatFS;
	FIL records;
	FRESULT res_mount, res_open;

	cTWI ioExpander(0x4E);

	cIOPin redLED(&PORTD, 5, cIOPin::output);
	cIOPin yellowLED(&PORTD, 6, cIOPin::output);
	cIOPin greenLED(&PORTD, 7, cIOPin::output);

	cADCPin tensiometer(0);
	cLCD userDisplay(&ioExpander);


	cDeviceRTC ds1302(&rtcCE, &rtcIO, &rtcSCLK);
	ds1302.set_RTC(2018, 9, 19, 9, 50, 0);

	res_mount = f_mount(&fatFS, "", 0);
	if(res_mount == FR_OK)
	{
		_delay_ms(10);
		res_open = f_open(&records, "records.txt", FA_WRITE | FA_OPEN_ALWAYS | FA_READ);
		if(res_open == FR_OK)
		{
			_delay_ms(10);
			f_puts(header0, &records);
			f_putc(',', &records);
			f_puts(header1, &records);
			f_putc(',', &records);
			f_puts(header2, &records);
			f_putc(',', &records);
			f_puts(header3, &records);
			f_putc('\n', &records);

		}
		f_close(&records);
	}

	userDisplay.SetCursor(5,1);
	userDisplay.WriteString("DripMatic");
	userDisplay.SetCursor(7, 2);
	userDisplay.WriteString("ver.0.1");
	_delay_ms(3000);
	userDisplay.Clear();

	uint16_t currentValue { 0 };
	uint16_t lastValue { 0 };

	for(;;)
	{

		greenLED.SetPin(1);
		ds1302.update_rtcTime();

		DisplayDate(&userDisplay, &ds1302);
		DisplayTime(&userDisplay, &ds1302);

		userDisplay.WriteStringXY(0, 2, "last: ");
		userDisplay.WriteInt(lastValue);
		userDisplay.WriteStringXY(0, 3, "current: ");
		userDisplay.WriteInt(currentValue);

		if(!sensorReading){
			_delay_ms(30000);
			sensorReading = true;
		}

		if( ( (ds1302.rtcTime.minutes % 15 ) == 0) && (sensorReading) )
		{
			userDisplay.Clear();
			greenLED.SetPin(0);
			lastValue = currentValue;
			uint16_t adcValue = 0;
			uint8_t count  = 0;
			while(count < 50)
			{
				userDisplay.WriteStringXY(0, 0, "reading sensor...");
				yellowLED.TogglePin();
				adcValue += tensiometer.Read();
				count++;
				_delay_ms(10);
			}
			currentValue = adcValue / 50;

			userDisplay.WriteStringXY(0, 0, "Tensiometer-Value");
			userDisplay.WriteStringXY(0, 2, "last: ");
			userDisplay.WriteInt(lastValue);
			userDisplay.WriteStringXY(0, 3, "current: ");
			userDisplay.WriteInt(currentValue);
			_delay_ms(10000);

			res_open = open_append(&records, "records.txt");
			if(res_open == FR_OK)
			{
				itoa(runningNumber, convertBuffer, 10);
				f_puts(convertBuffer, &records);
				f_putc(',', &records);
				ds1302.update_rtcTime();
				itoa(ds1302.rtcTime.year, convertBuffer, 10);
				f_puts(convertBuffer, &records);
				f_putc('/', &records);
				itoa(ds1302.rtcTime.month, convertBuffer, 10);
				f_puts(convertBuffer, &records);
				f_putc('/', &records);
				itoa(ds1302.rtcTime.date, convertBuffer, 10);
				f_puts(convertBuffer, &records);
				f_putc(',', &records);
				itoa(ds1302.rtcTime.hours, convertBuffer, 10);
				f_puts(convertBuffer, &records);
				f_putc(':', &records);
				itoa(ds1302.rtcTime.minutes, convertBuffer, 10);
				f_puts(convertBuffer, &records);
				f_putc(':', &records);
				itoa(ds1302.rtcTime.seconds, convertBuffer, 10);
				f_puts(convertBuffer, &records);
				f_putc(',', &records);
				itoa(currentValue, convertBuffer, 10);
				f_puts(convertBuffer, &records);
				f_putc('\n', &records);
			}
			f_close(&records);

			userDisplay.Clear();
			greenLED.SetPin(1);
			sensorReading = false;
			runningNumber++;
		}
	}
}

void DisplayDate(cLCD *lcd, cDeviceRTC *clock)
{
	lcd->SetCursor(0, 0);
	// frame_date
	lcd->WriteInt(clock->rtcTime.year);
	lcd->WriteString("/");
	if(clock->rtcTime.month < 10)
	{
		 lcd->WriteString("0");
		 lcd->WriteInt(clock->rtcTime.month);
	}
	else
	{
		lcd->WriteInt(clock->rtcTime.month);
	}
	lcd->WriteString("/");
	if(clock->rtcTime.date < 10)
	{
		 lcd->WriteString("0");
		 lcd->WriteInt(clock->rtcTime.date);
	}
	else
	{
		lcd->WriteInt(clock->rtcTime.date);
	}
}

void DisplayTime(cLCD *lcd, cDeviceRTC *clock)
{
	lcd->SetCursor(15, 0);
	if(clock->rtcTime.hours < 10)
	{
		lcd->WriteString("0");
		lcd->WriteInt(clock->rtcTime.hours);
		lcd->WriteString(":");
	}
	else
	{
		lcd->WriteInt(clock->rtcTime.hours);
		lcd->WriteString(":");
	}
	if(clock->rtcTime.minutes < 10)
	{
		lcd->WriteString("0");
		lcd->WriteInt(clock->rtcTime.minutes);
	}
	else
	{
		lcd->WriteInt(clock->rtcTime.minutes);
	}
}
