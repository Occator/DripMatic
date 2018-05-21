#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "pin_io.h"
#include "spi_module.h"
#include "sdcard_module.h"
#include "uart_module.h"


int main(){
	cUART sdComm;
	cIOPin spiCS(&PORTB, 2, cIOPin::output);
	cSPIModule sdSPI;
	cMicroSDModule microSD(&sdComm, &spiCS, &sdSPI);


	for(;;)
	{

	}
}
