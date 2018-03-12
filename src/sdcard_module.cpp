#include "sdcard_module.h"

cSDCardModule::cSDCardModule(cSPIModule * csDevice, cUART *uartComm)
: _spi(csDevice), _displaySD(uartComm)
{
  _displaySD->write_String("init SD-Card...");
  init_SPIMode();
  _displaySD->write_String("successful...");
}

cSDCardModule::~cSDCardModule()
{

}

void cSDCardModule::init_SPIMode()
{

}
