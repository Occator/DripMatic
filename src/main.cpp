#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "pin_io.h"
#include "spi_module.h"
#include "sdcard_module.h"
#include "uart_module.h"

cUART sdComm;
cIOPin spiCS(&PORTB, 1, cIOPin::output);
cSPIModule sdSPI(&spiCS);
cMicroSDModule microSD(&sdComm, &spiCS, &sdSPI);

int main(){

	for(;;)
	{

	}
}
