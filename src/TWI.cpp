// tested and worked at 2017/09/28 22:06

#include "TWI.h"


cTWI::cTWI(uint8_t devAddress) : _devAddr(devAddress)
{
	init();
}
cTWI::~cTWI(){}

void cTWI::transmit(uint8_t data)
{
	start();
	write(data);
	stop();
}

uint8_t cTWI::receive()
{
	uint8_t rByte = 0;
	start();
	rByte = read();
	stop();
	return rByte;
}

void cTWI::init()
{
	TWSR = 0;
	TWBR = (uint8_t)TWBR_VALUE;
}

void cTWI::start()
{
	TWCR = 0;
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while( !(TWCR & (1 << TWINT)) );
	TWDR = _devAddr;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while( !(TWCR & (1 << TWINT)) );
}

void cTWI::stop()
{
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void cTWI::write(uint8_t data)
{
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while( !(TWCR & (1 << TWINT) ) );
}

uint8_t cTWI::read()
{
	TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
	while (! (TWCR & (1<<TWINT) ) );
	return TWDR;
}
