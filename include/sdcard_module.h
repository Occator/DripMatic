#ifndef SDCARD_MODULE_H
#define SDCARD_MODULE_H

#include "spi_module.h"

// formular for CMDs - command number n + 64 = 64 = 0x40
#define CMD0 0x40

class cSDCardModule
{
public:
  cSDCardModule(cSPIModule *spiModule, cUART *uartComm);
  ~cSDCardModule();


private:
  cSPIModule *_spi;
  cUART *_displaySD;

  void init_SPIMode();
  void send_dummyByte();
  void send_Cmd(uint8_t command);
};

#endif
