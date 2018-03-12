#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "pin_io.h"
#include "spi_module.h"
#include "sdcard_module.h"
#include "uart_module.h"

cIOPin spiCS(&PORTB, 1, cIOPin::output);
cUART sdComm;
cSPIModule sdCard(&spiCS);
cSDCardModule microSD(&sdCard, &sdComm);

int main(){

	for(;;)
	{

	}
}
