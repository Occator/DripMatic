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

	cIOPin chipSelect(&PORTB, 1, cIOPin::output);

	cSPIModule microSDCard(&chipSelect);
	uint8_t dataout;

	for(;;)
	{
		microSDCard.transmit('S');
		microSDCard.transmit("TBan");
		_delay_ms(5);
	}
}
