#include "sdcard_module.h"

cSDCardModule::cSDCardModule(cSPIModule * spiModule) : _spi(spiModule)
{
  init();
}

cSDCardModule::~cSDCardModule()
{

}

void cSDCardModule::init()
{
  
}
