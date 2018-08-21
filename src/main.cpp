#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "sdcard_module.h"
#include "uart_module.h"

int main(){
	cUART::getInstance()->init();
	cUART::getInstance()->write_String("-----------------------------------\r\n");
	cUART::getInstance()->write_String("testing UART singleton\r\n");


	_delay_ms(5000);
	cIOPin spiCS(&PORTB, 2, cIOPin::output);
	cSPIModule sdSPI;
	cMicroSDModule microSD(&spiCS, &sdSPI);


	uint8_t rwBuffer[512];
	cUART::getInstance()->write_String("clear r/w buffer\r\n");
	for(uint16_t j = 0; j < 512; j++)
	{
		rwBuffer[j] = 0;
	}

	cUART::getInstance()->write_String("fill r/w buffer with 128 A's\r\n");
	for(uint8_t i = 0; i < 128; i++)
	{
		rwBuffer[i] = 'A';
	}

	cUART::getInstance()->write_String("write single Block\r\n");
	microSD.writeSingeBlock(rwBuffer, 4);

	_delay_ms(2000);

	cUART::getInstance()->write_String("read single Block\r\n");
	microSD.readSingleBlock(rwBuffer, 4);

	for(uint16_t j = 0; j < 512; j++)
	{
		cUART::getInstance()->write_Int(rwBuffer[j]);
		cUART::getInstance()->write_String("\r\n");
	}
	cUART::getInstance()->write_String("-----------------------------------\r\n");

	for(;;)
	{
	}
}
