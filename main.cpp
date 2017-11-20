// tested and worked at 2017/09/28 22:06

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "TWI/class_TWI.hpp"
#include "TWI/class_lcd_1602.hpp"


int main(){

	cTWI twiIOexpander(0x4E);
	cLCD1602 lcdTWI(&twiIOexpander);
	lcdTWI.init();

	lcdTWI.write_String_XY(0, 1, "hey pat");
	_delay_ms(2000);
	lcdTWI.write_String_XY(0, 2, "start while-loop");

	for(;;);
}

