// tested and worked at 2017/09/28 22:06

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "TWI/class_TWI.hpp"
#include "TWI/class_lcd_1602.hpp"
#include "ADC/class_adc_pin.hpp"
#include "MCU/class_pin_io.hpp"


int main(){

	cTWI twiIOexpander(0x4E);
	cLCD1602 lcdTWI(&twiIOexpander);

	cIOPin redLED(&PORTB, 4, cIOPin::output);
	redLED.set_Pin(1);
	cIOPin yellowLED(&PORTB, 3, cIOPin::output);
	redLED.set_Pin(1);
	cIOPin greenLED(&PORTB, 2, cIOPin::output);
	redLED.set_Pin(1);

	cADCPin LDR(0);
	LDR.init();
	// temp sensor
	cADCPin LM35(2);
	LM35.init();

	lcdTWI.init();

	lcdTWI.clear();
	lcdTWI.write_String("testing ADC");
	_delay_ms(3000);

	lcdTWI.no_Blink();
	lcdTWI.no_Cursor();

	int8_t count = 0;

	for(;;)
	{
		if(count > 4)	yellowLED.set_Pin(0);
		else yellowLED.set_Pin(1);
		lcdTWI.clear();
		lcdTWI.write_String_XY(4, 1, "ADC-Value");
		auto adcValue = LDR.read();
		lcdTWI.set_Cursor(7, 2);
		lcdTWI.write_Int(adcValue);
		_delay_ms(500);
		lcdTWI.write_String_XY(2, 1, "Temperature");
		auto tempValue = LM35.read();
		lcdTWI.set_Cursor(7, 2);
		lcdTWI.write_Int(tempValue);
		_delay_ms(500);
		redLED.set_Pin(0);
		yellowLED.set_Pin(0);
		_delay_ms(500);
		count++;
	}
}
