#include "pin_io.h"

cIOPin::cIOPin() : _bitVal(_defaultInit)
{}

cIOPin::cIOPin(volatile uint8_t *port, const uint8_t bit, eDirection direction) : _port(port), _bitVal(bit), _dir(direction){
	switch(_dir){
		case 0: *(port - 1) &= ~(1 << bit);
		break;
		case 1: *(port - 1) |= (1 << bit);
		break;
	}
}

cIOPin::~cIOPin(){}

void cIOPin::SetPin(bool value)
{
	if(value != 0){
		*_port |= (1 << _bitVal);
	}else{
		*_port &= ~(1 << _bitVal);
	}
}

uint8_t cIOPin::GetPin()
{
	return ((*(_port - 2)) & (1 << _bitVal));
}

void cIOPin::TogglePin()
{
	*_port ^= (1 << _bitVal);
}

void cIOPin::ToggleDirection()
{
	*(_port - 1) ^= (1 << _bitVal);
}

void cIOPin::SetDirection(bool direction)
{
	switch(direction){
		case 0: *(_port - 1) &= ~(1 << _bitVal);
		break;
		case 1: *(_port -1) |= (1 << _bitVal);
		break;
	}
}
