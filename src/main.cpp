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

	 FRESULT fr;
	 FATFS fs;
	 FIL fil;
	 cUART comm;

	 /* Open or create a log file and ready to append */
	 f_mount(&fs, "", 0);

	 fr = f_open(&fil, "logfile.txt", FA_WRITE | FA_OPEN_ALWAYS);
	 if (fr != FR_OK) return 1;

	 comm.write_String("file opened\r\n");

	 /* Close the file */
	 f_close(&fil);

	 comm.write_String("file closed\r\n");

	for(;;)
	{
	}
}
