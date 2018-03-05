#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "TWI.h"
#include "lcd_1602.h"
#include "adc_pin.h"
#include "pin_io.h"
#include "rtc_3w.h"
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
