#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "twi_module.h"
#include "lcd_module.h"
#include "adc_pin.h"
#include "pin_io.h"
#include "rtc_3w.h"
#include "spi_module.h"

int main(){

	cSPIModule microSDCard;
	uint8_t data = 'A';

	for(;;)
	{
		microSDCard.send_byte(data);
		_delay_ms(1000);
	}
}
