#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "sdcard_module.h"
#include "uart_module.h"
#include "diskio.h"
#include "ff.h"

int main(){

	cUART::getInstance()->init();
	cUART::getInstance()->write_String("-----------------------------------\r\n");
	cUART::getInstance()->write_String("testing avr to SD-Card\r\n");
	cUART::getInstance()->write_String("\r\n");

	 FATFS fatFs;
	 FIL fil;
	 FRESULT res_mount, res_open;

	 _delay_ms(10);

	 // Open or create a log file and ready to append
	 res_mount = f_mount(&fatFs, "", 0);

	 if(res_mount == FR_OK)
	 {
		 cUART::getInstance()->write_String("FAT mounted\r\n");
		 _delay_ms(10);
		 res_open = f_open(&fil, "test.txt", FA_WRITE | FA_OPEN_ALWAYS | FA_READ);

		 if(res_open == FR_OK)
		 {
			 cUART::getInstance()->write_String("file open\r\n");
			 _delay_ms(10);
			 cUART::getInstance()->write_String("writing to file ...\r\n");
			 _delay_ms(10);
			 f_printf(&fil, "testing on avr 1, 2, 3, ...\n");
		 }
		 f_close(&fil);
		 cUART::getInstance()->write_String("file closed\r\n");
		}

		res_open = open_append(&fil, "test.txt");
		if(res_open == FR_OK)
		{
			cUART::getInstance()->write_String("file open append\r\n");
			cUART::getInstance()->write_String("writing to file ...\r\n");
			f_printf(&fil, "file open append ...\n");
		}
		f_close(&fil);
		cUART::getInstance()->write_String("file closed\r\n");


	for(;;)
	{
	}
}
