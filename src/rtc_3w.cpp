#include "rtc_3w.h"

cDeviceRTC::cDeviceRTC()
{}

cDeviceRTC::~cDeviceRTC()
{}

cDeviceRTC& cDeviceRTC::GetInstance()
{
	static cDeviceRTC rtcModule;
	return rtcModule;
}

void cDeviceRTC::init(cIOPin *cePin, cIOPin *ioPin, cIOPin *sclkPin)
{
	_rtcCE = cePin;
	_rtcIO = ioPin;
	_rtcSCLK = sclkPin;

	write_RTC_Reg(0, cDeviceRTC::write_protect);
	set_RTC(2018, 9, 4, 20, 50, 0);
}
void cDeviceRTC::update_rtcTime()
{
	rtcTime.year = ( read_RTC_Reg(cDeviceRTC::year) + 2000);
	rtcTime.date = read_RTC_Reg(cDeviceRTC::date);
	rtcTime.month = read_RTC_Reg(cDeviceRTC::month);
	rtcTime.hours = read_RTC_Reg(cDeviceRTC::hour);
	rtcTime.minutes = read_RTC_Reg(cDeviceRTC::min);
	rtcTime.seconds = read_RTC_Reg(cDeviceRTC::sec);
	// rest of rtcTime needs to be implemented
}

void cDeviceRTC::set_RTC(uint16_t year, uint8_t month, uint8_t date,
												uint8_t hour, uint8_t minute,	uint8_t second)
{
	write_RTC_Reg( (year - 2000), cDeviceRTC::year);
	write_RTC_Reg(date, cDeviceRTC::date);
	write_RTC_Reg(month, cDeviceRTC::month);
	write_RTC_Reg(hour, cDeviceRTC::hour);
	write_RTC_Reg(minute, cDeviceRTC::min);
	write_RTC_Reg(second, cDeviceRTC::sec);

}

void cDeviceRTC::write_RTC_Reg(uint8_t value, eRegister _reg)
{
	write_CommByte( 0x80 | ((static_cast<uint8_t>(_reg)) << 1) );
	value = dec_To_BCD(value);
	write_Byte(value);
}

uint8_t cDeviceRTC::read_RTC_Reg(eRegister _reg)
{
	write_CommByte( 0x81 | ((static_cast<uint8_t>(_reg)) << 1)  );
	uint8_t bcdData = read_Byte();
	return bcd_To_Dec(bcdData);
}

void cDeviceRTC::write_CommByte(uint8_t reg)
{
	_rtcIO->SetDirection(cIOPin::output);
	_rtcCE->SetPin(1);
	_delay_ms(10);

	for(uint8_t i = 0; i < 8; i++){
		_rtcSCLK->SetPin(0);
		if(reg & (1 << i)){
			_rtcIO->SetPin(1);
		}else{
			_rtcIO->SetPin(0);
		}
		_rtcSCLK->SetPin(1);
	}
}

void cDeviceRTC::write_Byte(uint8_t data)
{
	_rtcSCLK->SetPin(0);
	for(uint8_t i = 0; i < 8; i++){
		if(data & (1 << i)){
			_rtcIO->SetPin(1);
		}else{
			_rtcIO->SetPin(0);
		}
		_rtcSCLK->SetPin(1);
		_rtcSCLK->SetPin(0);
	}

	_rtcCE->SetPin(0);
}


uint8_t cDeviceRTC::read_Byte()
{
	uint8_t byte {0};

	// set DDRD IO as INPUT
	_rtcIO->SetDirection(cIOPin::input);

	// set IO and SCLK low
	_rtcIO->SetPin(0);
	_rtcSCLK->SetPin(0);

	_delay_ms(50);

	for(uint8_t i = 0; i < 8; i++){
		if(PIND & (1 << 3)){
			byte |= (1 << i);
		}
		_rtcSCLK->SetPin(1);
		_rtcSCLK->SetPin(0);
	}

	_rtcCE->SetPin(0);
	return byte;
}


uint8_t cDeviceRTC::bcd_To_Dec(uint16_t bcdByte)
{
	return(((bcdByte & 0xf0) >> 4) * 10) + (bcdByte & 0x0f);
}

uint8_t cDeviceRTC::dec_To_BCD(uint16_t decByte)
{
	return (((decByte / 10) << 4) | (decByte % 10));
}
