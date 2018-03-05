#include <avr/io.h>
#include <stdint.h>
#include "class_adc_pin.h"


cADCPin::cADCPin(uint8_t ch)
{
	_channel = ch;
	// keep channel in range
	_channel &= 0x07;
	init();
}
cADCPin::~cADCPin(){}

void cADCPin::init()
{
	// set ref. Voltage [ 5V ]
	ADMUX |= (1 << REFS0);
	// clear ADC channel selection
	ADMUX &= ~( (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0) );
	// set prescaler 128
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	// enable auto-trigger and select auto-trigger source
	//ADCSRA |= (1 << ADATE) | (1 << ADIE);
	//ADCSRB |= (1 << ADTS1);  // External Interrupt Request 0 ADTS[2:0] 0-1-0
	// enable ADC
	ADCSRA |= (1 << ADEN);
	// start first conversion and make a "dummy-readout"
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC)){}
}

uint16_t cADCPin::read()
{
	ADMUX &= ~( (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0) );
	// select channel
	ADMUX |= _channel;
	// start conversion
	ADCSRA |= (1 << ADSC);
	// wait until conversion is completed
	while(ADCSRA & (1 << ADSC)){}
		return ADC;
}
