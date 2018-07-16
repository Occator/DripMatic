#include "diskio.h"
#include "sdcard_module.h"
#include "uart_module.h"
#include "pin_io.h"
#include "spi_module.h"


static cMicroSDModule* sdCard_ = 0;

static cUART uart;
static cSPIModule spi;
static cIOPin chipSelect(&PORTB, 2, cIOPin::output);
static cMicroSDModule sdCard(&uart, &chipSelect, &spi);

extern "C"
{
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	if( sdCard_ != 0 )
		return 0;
	return STA_NOINIT;
}

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
  sdCard_ = &sdCard;

	uart.write_String("SD-Card successful initialized\r\n");
  return RES_OK;
}

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
  if( sdCard_ == 0 )
    return RES_PARERR;
  for(uint16_t i = 0; i < count; ++i)
      sdCard_->readSingleBlock(buff+i*BLOCK_LENGTH, sector);
  return RES_OK;
}

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
  if( sdCard_ == 0 )
    return RES_PARERR;
  for( uint16_t i = 0; i < count; ++i)
    sdCard_->writeSingeBlock((uint8_t*)buff+i*BLOCK_LENGTH, sector);
  return RES_OK;
}

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	return RES_PARERR;
}
}
