// tested and worked at 2017/09/21 15:45

#ifndef RTC_3W__H
#define RTC_3W__H

#include <avr/io.h>
#include <util/delay.h>
#include "pin_io.h"

class cSysTime{
public:
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t date;
	uint8_t month;
	uint16_t dow;
	uint16_t year;
};

class cDeviceRTC{
public:

	enum eRegister
	{
		sec = 0, min, hour, date, month, dow, year, write_protect
	};

	struct cTime
	{
		uint8_t seconds;
		uint8_t minutes;
		uint8_t hours;
		uint8_t date;
		uint8_t month;
		uint16_t dow;
		uint16_t year;

	}rtcTime;

	cDeviceRTC(cIOPin *cePin, cIOPin *ioPin, cIOPin *sclkPin);
	~cDeviceRTC();
	void update_rtcTime();
	void set_RTC(uint16_t year, uint8_t month, uint8_t date, uint8_t hour, uint8_t minute, uint8_t second);

private:
	void write_RTC_Reg(uint8_t value, eRegister _reg);
	uint8_t read_RTC_Reg(eRegister _reg);
	void write_CommByte(uint8_t reg);
	void write_Byte(uint8_t data);
	uint8_t read_Byte();
	uint8_t bcd_To_Dec(uint16_t bcdByte);
	uint8_t dec_To_BCD(uint16_t decByte);

	eRegister _reg;
	// ## implement smart pointer, if there is a workaround, because no standard memory.h
	cIOPin *_rtcCE;
	cIOPin *_rtcIO;
	cIOPin *_rtcSCLK;
};

#endif
