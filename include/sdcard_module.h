#ifndef SDCARD_MODULE_H
#define SDCARD_MODULE_H

#include "spi_module.h"
#include "pin_io.h"

// formular for CMDs - command number n + 64 = 64 = 0x40
#define CMD0 0x40

class cSDCardModule
{
public:
  cSDCardModule(cUART *uartComm, cSPIModule * csDevice);
  ~cSDCardModule();


private:
  cUART *_uartSD;
  cSPIModule *_spi;

  void init_SPIMode();
  void send_dummyByte();
  void send_Cmd(uint8_t command);
};

#endif
