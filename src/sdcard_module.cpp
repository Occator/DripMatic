#include "sdcard_module.h"

cSDCardModule::cSDCardModule(cSPIModule * csDevice, cUART *uartComm)
: _spi(csDevice), _displaySD(uartComm)
{
  _displaySD->write_String("init SD-Card...");
  init();
}

cSDCardModule::~cSDCardModule()
{

}

void cSDCardModule::init()
{

}
