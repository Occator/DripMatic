#ifndef SDCARD_MODULE_H
#define SDCARD_MODULE_H

#include "spi_module.h"
#include "pin_io.h"

// formular for CMDs - command number n | 0x40
#define GO_IDLE_STATE 0
#define SEND_OP_COND 1
#define SEND_IF_COND 8
#define SEND_CSD 9
#define STOP_TRANSMISSION 12
#define SEND_STATUS 13
#define SET_BLOCK_LEN 16
#define READ_SINGLE_BLOCK 17
#define WRITE_SINGLE_BLOCK 24
#define SD_SEND_OP_COND 41 // ACMD
#define APP_CMD	55
#define READ_OCR 58
#define CRC_ON_OFF 59


class cMicroSDModule
{
public:
  cMicroSDModule(cUART *uartComm, cIOPin *chipSelect, cSPIModule * csDevice);
  ~cMicroSDModule();


private:
  cUART *_uartSD;
  cIOPin *_csPin;
  cSPIModule *_spi;

  void init_SPIMode();
  uint8_t send_Commandmd(uint8_t command, uint32_t argument);
};

#endif
