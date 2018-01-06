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

	cIOPin redLED(&PORTB, 4, cIOPin::output);
	cIOPin yellowLED(&PORTB, 3, cIOPin::output);
	cIOPin greenLED(&PORTB, 2, cIOPin::output);


	// define RTC control pins
	cIOPin rtcCE(&PORTD, 2, cIOPin::output);
	cIOPin rtcIO(&PORTD, 3, cIOPin::output);
	cIOPin rtcSCLK(&PORTD, 4, cIOPin::output);

	cDeviceRTC ds1302(&rtcCE, &rtcIO, &rtcSCLK);
	ds1302.set_RTC(2017, 25, 11, 14, 14, 45);

	cADCPin LDR(0);
	cADCPin TMP(2);

	cLCD1602 lcdTWI(&twiIOexpander);
	cLCD1602 lcdFrameDate(&twiIOexpander, 0, 0, ds1302.rtcTime.year);

	lcdTWI.init();

	lcdTWI.clear();
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
		// frame_date
		lcdTWI.set_Cursor(0, 0);
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
		// frame_time
		lcdTWI.set_Cursor(12, 0);
		if(ds1302.rtcTime.hours < 10)
		{
			lcdTWI.write_String("0");
			lcdTWI.write_Int(ds1302.rtcTime.hours);
			lcdTWI.write_String(":");
		}
		else
		{
			lcdTWI.write_Int(ds1302.rtcTime.hours);
			lcdTWI.write_String(":");
		}
		if(ds1302.rtcTime.minutes < 10)
		{
			lcdTWI.write_String("0");
			lcdTWI.write_Int(ds1302.rtcTime.minutes);
			lcdTWI.write_String(":");
		}
		else
		{
			lcdTWI.write_Int(ds1302.rtcTime.minutes);
			lcdTWI.write_String(":");
		}
		if(ds1302.rtcTime.seconds < 10)
		{
			lcdTWI.write_String("0");
			lcdTWI.write_Int(ds1302.rtcTime.seconds);
		}
		else
		{
			lcdTWI.write_Int(ds1302.rtcTime.seconds);
		}
		// frame_temperature
		uint32_t tempValue = TMP.read();
		tempValue /= 4;
		if(tempValue < 10)
		{
			lcdTWI.write_String_XY(0, 2, "T:");
			lcdTWI.write_Int(tempValue);
			lcdTWI.write(0xDF);
			lcdTWI.write_String("C ");
		}
		else
		{
		lcdTWI.write_String_XY(0, 2, "T:");
		lcdTWI.write_Int(tempValue);
		lcdTWI.write(0xDF);
		lcdTWI.write('C');
		}

		// frame_relativeHumidity
		uint8_t rhValue = 40;
		if(rhValue < 10)
		{
			lcdTWI.write_String_XY(14, 2, " rh:");
			lcdTWI.write_Int(rhValue);
			lcdTWI.write('%');
		}
		else
		{
		lcdTWI.write_String_XY(14, 2, "rh:");
		lcdTWI.write_Int(rhValue);
		lcdTWI.write('%');
		}

		// frame_soilMoisture
		if(sensorValue < 10)
		{
			lcdTWI.write_String_XY(0, 3, "SM:");
			lcdTWI.write_Int(sensorValue);
			lcdTWI.write('%');
			lcdTWI.write(' ');
		}
		else
		{
		lcdTWI.write_String_XY(0, 3, "SM:");
		lcdTWI.write_Int(sensorValue);
		lcdTWI.write('%');
		}


		if(ds1302.rtcTime.minutes == 0 || ds1302.rtcTime.minutes == 15 || ds1302.rtcTime.minutes == 30 || ds1302.rtcTime.minutes == 45)
		{
			lcdTWI.clear();
			lcdTWI.write_String("start");
			lcdTWI.write_String_XY(0, 2, "measurement...");
			_delay_ms(1000);
			uint8_t count = 0;
			uint32_t adcValue = 0;


			while(count < 60)
			{
				yellowLED.set_Pin(1);
				adcValue = adcValue + LDR.read();
				count++;
				_delay_ms(500);
				yellowLED.set_Pin(0);
				_delay_ms(500);
			}
			sensorValue = adcValue / 60;


			lcdTWI.clear();
			lcdTWI.write_String_XY(2, 1, "Sensor value");
			lcdTWI.set_Cursor(7, 2);
			lcdTWI.write_Int(sensorValue);
			_delay_ms(1000);
			lcdTWI.clear();

			while(sensorValue < 19)
			{
				lcdTWI.write_String("start");
				lcdTWI.write_String_XY(0, 2, "irrigation...");
				_delay_ms(2000);
				uint32_t irrigationValue = 22;
				redLED.set_Pin(1);
				while(sensorValue < irrigationValue){
					lcdTWI.clear();
					lcdTWI.write_String("irrigation");
					lcdTWI.write_String_XY(0, 2, "needed");
					_delay_ms(1000);
					sensorValue = LDR.read();
				}
				redLED.set_Pin(0);
				lcdTWI.clear();
			}
		}
	}
}
