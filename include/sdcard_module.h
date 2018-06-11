#ifndef SDCARD_MODULE_H
#define SDCARD_MODULE_H

#include "spi_module.h"
#include "pin_io.h"

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
#define WRITE_SINGLE_BLOCK 24
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
  uint8_t readSingleBlock(uint32_t startBlock);
  void readOCRRegister(uint8_t *buffer);
  void getRWBuffer(uint8_t *rwBuffer);
  uint8_t firstDataByte {0};
  uint8_t secondDataByte {0};
  uint8_t thirdDataByte {0};

private:
  cUART *_uartSD;
  cIOPin *_csPin;
  cSPIModule *_spi;
  bool _isSuccessful = false;
  uint8_t _rwBuffer[BLOCK_LENGTH];
  uint8_t _registerBuffer[48];
  uint8_t _ocrRegister[5];

  void _csAsserted();
  void _csDeasserted();
  uint8_t _initSPIMode();
  uint8_t _sendAppCmd();
  uint8_t _sendOCRCmd();
};

#endif
