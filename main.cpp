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


	lcdTWI.set_Cursor(0, 1);
	lcdTWI.write_String("hey pat");
	_delay_ms(2000);
	lcdTWI.set_Cursor(0, 2);
	lcdTWI.write_String("ich bin es");
	lcdTWI.clear();

	for(;;);
}
