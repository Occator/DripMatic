#ifndef SDCARD_MODULE_H
#define SDCARD_MODULE_H

#include "spi_module.h"

class cSDCardModule
{
public:
  cSDCardModule(cSPIModule *spiModule, cUART *uartComm);
  ~cSDCardModule();


private:
  cSPIModule *_spi;
  cUART *_displaySD;

  void init();
};

#endif
