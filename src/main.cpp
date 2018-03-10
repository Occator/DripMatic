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
	uint8_t datain = 0;
	uint8_t dataout;

	microSDCard.master_init();

	for(;;)
	{
		PORTB |= (1 << PB1);
		microSDCard.send_byte(datain);
		dataout = microSDCard.receive_byte();
		PORTB &= ~(1 << PB1);
		_delay_ms(5);
		datain++;
	}
}
