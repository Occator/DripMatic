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

	uint8_t sdRegBuffer[5];

	microSD.readOCRRegister(sdRegBuffer);

	sdComm.write_String("OCR-Register:\r\n");
	for(uint8_t i = 0; i < 4; i++)
	{
		sdComm.write_Int(sdRegBuffer[i]);
		sdComm.write_String("\r\n");
	}

	sdComm.write_String("r/w buffer content:\r\n");
	microSD.readSingleBlock(0);
	uint8_t rwBuffer[512];
	microSD.getRWBuffer(rwBuffer);
	for(uint8_t j = 0; j < 512; j++)
	{
		sdComm.write_Int(rwBuffer[j]);
		sdComm.write_String("\r\n");
	}


	for(;;)
	{
	}
}
