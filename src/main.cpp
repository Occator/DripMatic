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
	cUART::getInstance().init();
	cUART::getInstance().write_String("enter main\r\n");
	void DisplayDate(cLCD *lcd2004);
	void DisplayTime(cLCD *lcd2004);
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
	cDeviceRTC::GetInstance().init(&rtcCE, &rtcIO, &rtcSCLK);


	FATFS fatFS;
	FIL records;
	FRESULT res_mount, res_open;

	cTWI ioExpander(0x4E);

	cIOPin redLED(&PORTD, 5, cIOPin::output);
	cIOPin yellowLED(&PORTD, 6, cIOPin::output);
	cIOPin greenLED(&PORTD, 7, cIOPin::output);

	cADCPin tensiometer(0);
	cLCD userDisplay(&ioExpander);

	res_mount = f_mount(&fatFS, "", 0);
	if(res_mount == FR_OK)
	{
		_delay_ms(10);
		res_open = f_open(&records, "records.csv", FA_WRITE | FA_OPEN_ALWAYS | FA_READ);
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

	cUART::getInstance().write_String("enter infinite loop");
	for(;;)
	{

		greenLED.SetPin(1);
		cDeviceRTC::GetInstance().update_rtcTime();

		DisplayDate(&userDisplay);
		DisplayTime(&userDisplay);

		userDisplay.WriteStringXY(0, 2, "last: ");
		userDisplay.WriteInt(lastValue);
		userDisplay.WriteStringXY(0, 3, "current: ");
		userDisplay.WriteInt(currentValue);

		if(!sensorReading){
			_delay_ms(30000);
			sensorReading = true;
		}

		if( ( (cDeviceRTC::GetInstance().rtcTime.minutes % 15 ) == 0) && (sensorReading) )
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

			res_open = open_append(&records, "records.csv");
			if(res_open == FR_OK)
			{
				itoa(runningNumber, convertBuffer, 10);
				f_puts(convertBuffer, &records);
				f_putc(',', &records);
				cDeviceRTC::GetInstance().update_rtcTime();
				itoa(cDeviceRTC::GetInstance().rtcTime.year, convertBuffer, 10);
				f_puts(convertBuffer, &records);
				f_putc('/', &records);
				itoa(cDeviceRTC::GetInstance().rtcTime.month, convertBuffer, 10);
				f_puts(convertBuffer, &records);
				f_putc('/', &records);
				itoa(cDeviceRTC::GetInstance().rtcTime.date, convertBuffer, 10);
				f_puts(convertBuffer, &records);
				f_putc(',', &records);
				itoa(cDeviceRTC::GetInstance().rtcTime.hours, convertBuffer, 10);
				f_puts(convertBuffer, &records);
				f_putc(':', &records);
				itoa(cDeviceRTC::GetInstance().rtcTime.minutes, convertBuffer, 10);
				f_puts(convertBuffer, &records);
				f_putc(':', &records);
				itoa(cDeviceRTC::GetInstance().rtcTime.seconds, convertBuffer, 10);
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

void DisplayDate(cLCD *lcd)
{
	lcd->SetCursor(0, 0);
	// frame_date
	lcd->WriteInt(cDeviceRTC::GetInstance().rtcTime.year);
	lcd->WriteString("/");
	if(cDeviceRTC::GetInstance().rtcTime.month < 10)
	{
		 lcd->WriteString("0");
		 lcd->WriteInt(cDeviceRTC::GetInstance().rtcTime.month);
	}
	else
	{
		lcd->WriteInt(cDeviceRTC::GetInstance().rtcTime.month);
	}
	lcd->WriteString("/");
	if(cDeviceRTC::GetInstance().rtcTime.date < 10)
	{
		 lcd->WriteString("0");
		 lcd->WriteInt(cDeviceRTC::GetInstance().rtcTime.date);
	}
	else
	{
		lcd->WriteInt(cDeviceRTC::GetInstance().rtcTime.date);
	}
}

void DisplayTime(cLCD *lcd)
{
	lcd->SetCursor(15, 0);
	if(cDeviceRTC::GetInstance().rtcTime.hours < 10)
	{
		lcd->WriteString("0");
		lcd->WriteInt(cDeviceRTC::GetInstance().rtcTime.hours);
		lcd->WriteString(":");
	}
	else
	{
		lcd->WriteInt(cDeviceRTC::GetInstance().rtcTime.hours);
		lcd->WriteString(":");
	}
	if(cDeviceRTC::GetInstance().rtcTime.minutes < 10)
	{
		lcd->WriteString("0");
		lcd->WriteInt(cDeviceRTC::GetInstance().rtcTime.minutes);
	}
	else
	{
		lcd->WriteInt(cDeviceRTC::GetInstance().rtcTime.minutes);
	}
}
