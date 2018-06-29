#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "sdcard_module.h"
#include "uart_module.h"

int main(){
	cUART sdComm;
	cMicroSDModule microSD;


	uint8_t rwBuffer[512];

	for(uint16_t j = 0; j < 512; j++)
	{
		rwBuffer[j] = 0;
	}


	for(uint8_t i = 0; i < 8; i++)
	{
		rwBuffer[i] = 0;
	}

	microSD.writeSingeBlock(rwBuffer, 4);


	sdComm.write_String("r/w buffer content:\r\n");

	microSD.readSingleBlock(rwBuffer, 4);

	for(uint16_t j = 0; j < 512; j++)
	{
		sdComm.write_Int(rwBuffer[j]);
		sdComm.write_String("\r\n");
	}

	for(;;)
	{
	}
}
