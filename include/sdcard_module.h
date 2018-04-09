#ifndef SDCARD_MODULE_H
#define SDCARD_MODULE_H

#include "spi_module.h"
#include "pin_io.h"

// formular for CMDs - command number n + 64 = 64 = 0x40
#define CMD0 0x40

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
  void send_dummyByte();
  void send_Cmd(uint8_t command);
};

#endif
