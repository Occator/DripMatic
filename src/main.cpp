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

	FATFS fs;
	FIL fil;

	f_mount(&fs, "", 0);

	f_open(&fil, "logfile.txt", FA_WRITE | FA_OPEN_ALWAYS);
	f_close(&fil);

	for(;;)
	{
	}
}
