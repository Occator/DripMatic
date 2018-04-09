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
  uint8_t response, retry {0}, status;

  _csPin->set_Pin(1);

  _spi->transmit(command | 0x40);
  _spi->transmit(argument >> 24);
  _spi->transmit(argument >> 16);
  _spi->transmit(argument >> 8);
  _spi->transmit(argument);

  // it is recommended to send correct CRC for CMD8 (0x87) and CMD0 (0x95)
  if(command == SEND_IF_COND)
  {
    _spi->transmit(0x87);
  }
  else
  {
    _spi->transmit(0x95);
  }
  while( (response = _spi->receive() ) == 0xFF)
  {
    if(retry++ > 100) break;
  }
  _spi->receive();
  _csPin->set_Pin(0);
  return (response);
}
