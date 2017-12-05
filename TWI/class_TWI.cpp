// tested and worked at 2017/09/28 22:06

#include "class_TWI.hpp"


cTWI::cTWI(uint8_t devAddress) : _devAddr(devAddress){
	init();
}
cTWI::~cTWI(){}

void cTWI::init(){
	TWSR = 0;
	TWBR = (uint8_t)TWBR_VALUE;
}

void cTWI::start(){
	// reset status flag
	_startFlag = false;
	_slaAckFlag = false;
	TWCR = 0;
	// transmit START condition
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while( !(TWCR & (1 << TWINT)) );

	// check if start condition was successfully transmitted
	if( (TWSR & 0xF8) == TWI_START ){
		_startFlag = true;
	}

	// load slave address into data register
	TWDR = _devAddr;
	// start transmission of address
	TWCR = (1 << TWINT) | (1 << TWEN);
	while( !(TWCR & (1 << TWINT)) );

	// check if slave address was successfully transmitted
	if( (TWSR & 0xF8) == SLA_W_ACK ){
		_slaAckFlag = true;
	}
}

void cTWI::stop(){
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void cTWI::write(uint8_t data){
	// reset status flag
	_dataAckFlag = false;
	// load data into data register
	TWDR = data;
	// start transmission of data
	TWCR = (1 << TWINT) | (1 << TWEN);
	while( !(TWCR & (1 << TWINT)) );

	if( (TWSR & 0xF8) == DATA_ACK){
		_dataAckFlag = true;
	}
}

void cTWI::transmit(uint8_t data){
	start();
	write(data);
	stop();
}

bool cTWI::get_StartFlag()
{
	return _startFlag;
}

bool cTWI::get_SlaAckFlag()
{
	return _slaAckFlag;
}

bool cTWI::get_DataAckFlag()
{
	return _dataAckFlag;
}
// debug functions for interfacing with LCD
uint8_t cTWI::bcd_To_Dec(uint8_t bcdByte)
{
	return( ( (bcdByte & 0xf0) >> 4) * 10) + (bcdByte & 0x0f);
}

uint8_t cTWI::dec_To_BCD(uint8_t decByte)
{
	return ( ( (decByte / 10) << 4) | (decByte % 10));
}

uint32_t cTWI::bin_To_Hex(uint32_t binByte)
{
	uint32_t binNumber = 0;
	uint32_t hexNumber = 0;
	uint32_t j = 1; 
	uint32_t remain;

	binNumber = binByte;

	while(binNumber != 0)
	{
		remain = binNumber % 10;
		hexNumber = hexNumber + remain * j;
		j = j * 2;
		binNumber = binNumber / 10; 
	}
	return hexNumber;
}
//