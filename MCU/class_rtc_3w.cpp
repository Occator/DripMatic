// tested and worked at 2017/09/21 15:45

#include "class_rtc_3w.hpp"

cDeviceRTCcopy::cDeviceRTCcopy(cIOPin *cePin, cIOPin *ioPin, cIOPin *sclkPin) : _rtcCE(cePin), _rtcIO(ioPin), _rtcSCLK(sclkPin) {
	write_RTC_Reg(0, cDeviceRTCcopy::write_protect);
}

cDeviceRTCcopy::~cDeviceRTCcopy(){}

// implemented and tested

void cDeviceRTCcopy::set_RTC(uint16_t year, uint8_t date, uint8_t month, uint8_t hour, uint8_t minute, uint8_t second){
	write_RTC_Reg((year - 2000), cDeviceRTCcopy::year);
	write_RTC_Reg(date, cDeviceRTCcopy::date);
	write_RTC_Reg(month, cDeviceRTCcopy::month);
	write_RTC_Reg(hour, cDeviceRTCcopy::hour);
	write_RTC_Reg(minute, cDeviceRTCcopy::min);
	write_RTC_Reg(second, cDeviceRTCcopy::sec);

}

void cDeviceRTCcopy::write_RTC_Reg(uint8_t value, eRegister _reg){
	write_CommByte( 0x80 | ((static_cast<uint8_t>(_reg)) << 1) );
	value = dec_To_BCD(value);
	write_Byte(value);
}

void cDeviceRTCcopy::write_CommByte(uint8_t reg){
	_rtcIO->set_Direction(cIOPin::output);
	_rtcCE->set_Pin(1);
	_delay_ms(10);

	for(uint8_t i = 0; i < 8; i++){
		_rtcSCLK->set_Pin(0);
		if(reg & (1 << i)){
			_rtcIO->set_Pin(1);
		}else{
			_rtcIO->set_Pin(0);
		}
		_rtcSCLK->set_Pin(1);
	}
}

void cDeviceRTCcopy::write_Byte(uint8_t data){
	_rtcSCLK->set_Pin(0);
	for(uint8_t i = 0; i < 8; i++){
		if(data & (1 << i)){
			_rtcIO->set_Pin(1);
		}else{
			_rtcIO->set_Pin(0);
		}
		_rtcSCLK->set_Pin(1);
		_rtcSCLK->set_Pin(0);
	}

	_rtcCE->set_Pin(0);
}

uint8_t cDeviceRTCcopy::read_RTC_Reg(eRegister _reg){
	write_CommByte( 0x81 | ((static_cast<uint8_t>(_reg)) << 1)  );
	uint8_t bcdData = read_Byte();
	return bcd_To_Dec(bcdData);
}

uint8_t cDeviceRTCcopy::read_Byte(){
	uint8_t byte {0};

	// set DDRD IO as INPUT
	_rtcIO->set_Direction(cIOPin::input);

	// set IO and SCLK low
	_rtcIO->set_Pin(0);
	_rtcSCLK->set_Pin(0);

	_delay_ms(50);

	for(uint8_t i = 0; i < 8; i++){
		if(PIND & (1 << 3)){  //if(_rtcIO.get_Pin())
			byte |= (1 << i);
		}
		_rtcSCLK->set_Pin(1);
		_rtcSCLK->set_Pin(0);
	}

	_rtcCE->set_Pin(0);
	return byte;
}

void cDeviceRTCcopy::update_rtcTime(){
	rtcTime.year = ( read_RTC_Reg(cDeviceRTCcopy::year) + 2000);
	rtcTime.date = read_RTC_Reg(cDeviceRTCcopy::date);
	rtcTime.month = read_RTC_Reg(cDeviceRTCcopy::month);
	rtcTime.hours = read_RTC_Reg(cDeviceRTCcopy::hour);
	rtcTime.minutes = read_RTC_Reg(cDeviceRTCcopy::min);
	rtcTime.seconds = read_RTC_Reg(cDeviceRTCcopy::sec);
	// rest of rtcTime needs to be implemented
}

uint8_t cDeviceRTCcopy::bcd_To_Dec(uint16_t bcdByte){
	return(((bcdByte & 0xf0) >> 4) * 10) + (bcdByte & 0x0f);
}

uint8_t cDeviceRTCcopy::dec_To_BCD(uint16_t decByte){
	return (((decByte / 10) << 4) | (decByte % 10));
}
