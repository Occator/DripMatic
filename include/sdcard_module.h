#ifndef SDCARD_MODULE_H
#define SDCARD_MODULE_H

#include "spi_module.h"

class cSDCardModule
{
public:
  cSDCardModule(cSPIModule *spiModule);
  ~cSDCardModule();


private:
  cSPIModule *_spi
};

#endif
