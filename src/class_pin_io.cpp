// tested and worked at 2017/09/12 16:17

#include "class_pin_io.hpp"

extern "C" void __cxa_pure_virtual(){while(1);}

cIOPin::cIOPin(volatile uint8_t *port, const uint8_t bit, eDirection direction) : _port(port), _bitVal(bit), _dir(direction){
	switch(_dir){
		case 0: *(port - 1) &= ~(1 << bit);
		break;
		case 1: *(port - 1) |= (1 << bit);
		break;
	}
}


cIOPin::~cIOPin(){}

void cIOPin::set_Pin(bool value)
{
	if(value != 0){
		*_port |= (1 << _bitVal);
	}else{
		*_port &= ~(1 << _bitVal);
	}
}

uint8_t cIOPin::get_Pin()
{
	return ((*(_port - 2)) & (1 << _bitVal));
}

void cIOPin::toggle_Pin()
{
	*_port ^= (1 << _bitVal);
}

void cIOPin::toggle_Direction()
{
	*(_port - 1) ^= (1 << _bitVal);
}

void cIOPin::set_Direction(bool direction)
{
	switch(direction){
		case 0: *(_port - 1) &= ~(1 << _bitVal);
		break;
		case 1: *(_port -1) |= (1 << _bitVal);
		break;
	}
}
