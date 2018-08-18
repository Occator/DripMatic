#ifndef SDCARD_MODULE_H
#define SDCARD_MODULE_H

#include "spi_module.h"
#include "pin_io.h"
#include "uart_module.h"

// formular for CMDs - command number n | 0x40
#define GO_IDLE_STATE 0
#define SEND_OP_COND 1
#define SEND_IF_COND 8
#define SEND_CSD 9
#define SEND_CID 10
#define STOP_TRANSMISSION 12
#define SEND_STATUS 13
#define SET_BLOCK_LEN 16
#define READ_SINGLE_BLOCK 17
#define READ_MULTIPLE_BLOCKS 18
#define WRITE_SINGLE_BLOCK 24
#define WRITE_MULTIPLE_BLOCKS 25
#define SD_SEND_OP_COND 41 // ACMD
#define APP_CMD	55
#define READ_OCR 58
#define CRC_ON_OFF 59
#define BLOCK_LENGTH 512

// transmission
#define SD_START_TOKEN 0xFE

class cMicroSDModule
{
public:
  cMicroSDModule(cUART *uartComm, cIOPin *chipSelect, cSPIModule * csDevice);
  ~cMicroSDModule();

  uint8_t sendCommand(uint8_t command, uint32_t argument);
  uint8_t readSingleBlock(uint8_t *buffer, uint32_t startBlock);
  uint8_t writeSingeBlock(uint8_t *buffer, uint32_t startBlock);

private:
  cUART *_uartSD;
  cIOPin *_csPin;
  cSPIModule *_spi;
  bool _isSuccessful = false;

  void _csAsserted();
  void _csDeasserted();
  uint8_t _initSPIMode();
  uint8_t _sendAppCmd();
};

#endif
