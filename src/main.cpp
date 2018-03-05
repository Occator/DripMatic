#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "class_TWI.hpp"
#include "class_lcd_1602.hpp"
#include "class_adc_pin.hpp"
#include "class_pin_io.hpp"
#include "class_rtc_3w.hpp"
#include "spi_module.h"

int main(){

	cSPIModule microSDCard;
	uint8_t data = 'A';

	for(;;)
	{
		for(uint8_t i = 0; i < 26; i++)
		{
			microSDCard.send_byte(data);
			data++;
			_delay_ms(2000);
		}
	}
}
