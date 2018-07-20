#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "pin_io.h"
#include "spi_module.h"
#include "sdcard_module.h"
#include "uart_module.h"
#include "diskio.h"
#include "ff.h"


int main(){

	 FATFS fatFs;
	 FIL fil;
	 FRESULT res_mount, res_open, res_seek, res_write;
	 cUART comm;
	 uint16_t fa;

	 _delay_ms(10);

	 /* Open or create a log file and ready to append */
	 res_mount = f_mount(&fatFs, "", 1);

	 if(res_mount == FR_OK)
	 {
		 comm.write_String("FAT mounted\r\n");
		 _delay_ms(10);
		 res_open = f_open(&fil, "test.txt", FA_WRITE | FA_OPEN_ALWAYS | FA_READ);

		 if(res_open == FR_OK)
		 {
			 comm.write_String("file open\r\n");
			 _delay_ms(10);
			 res_seek = f_lseek(&fil, f_size(&fil) );
			 if(res_seek == FR_OK)
			 {
				 _delay_ms(10);

				 for(uint16_t count = 0; count < 1512; count++)
				 {
				 	res_write = f_write(&fil, "A", 1, &fa);
					if(fa > 0 && res_write == FR_OK)
					{
						f_sync(&fil);
						comm.write_String("buffer flush\r\n");
					}
			 	 }
				 f_close(&fil);
				 comm.write_String("file closed\r\n");
			 }
		 }
	 }

	for(;;)
	{
	}
}
