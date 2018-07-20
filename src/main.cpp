#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "sdcard_module.h"
#include "uart_module.h"

int main(){
	cUART sdComm;
	sdComm.write_String("testing sd card init, read/write single block:\r\n");

	_delay_ms(5000);
	cIOPin spiCS(&PORTB, 2, cIOPin::output);
	cSPIModule sdSPI;
	cMicroSDModule microSD(&sdComm, &spiCS, &sdSPI);


	uint8_t rwBuffer[512];

	for(uint16_t j = 0; j < 512; j++)
	{
		rwBuffer[j] = 0;
	}


	for(uint8_t i = 0; i < 8; i++)
	{
		rwBuffer[i] = 'A';
	}

	microSD.writeSingeBlock(rwBuffer, 4);


	_delay_ms(2000);

	sdComm.write_String("r/w buffer content:\r\n");
	microSD.readSingleBlock(rwBuffer, 5);

	for(uint16_t j = 0; j < 512; j++)
	{
		sdComm.write_Int(rwBuffer[j]);
		sdComm.write_String("\r\n");
	}

	for(;;)
	{
	}
}
