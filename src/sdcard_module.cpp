#include "sdcard_module.h"
#include <util/delay.h>

cMicroSDModule::cMicroSDModule(cUART *uartComm, cIOPin *chipSelect, cSPIModule * csDevice)
: _uartSD(uartComm), _csPin(chipSelect), _spi(csDevice)
{
  _uartSD->write_String("init SD-Card in SPI-mode...\r\n");
  init_SPIMode();
  _uartSD->write_String("successful...\r\n");
}

cMicroSDModule::~cMicroSDModule()
{}

void cMicroSDModule::init_SPIMode()
{

}

uint8_t cMicroSDModule::send_Command(uint8_t command, uint32_t argument)
{

}
