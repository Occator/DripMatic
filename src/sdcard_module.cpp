#include "sdcard_module.h"
#include <util/delay.h>

cSDCardModule::cSDCardModule(cSPIModule * csDevice, cUART *uartComm)
: _spi(csDevice), _displaySD(uartComm)
{
  _displaySD->write_String("init SD-Card in SPI-mode...\r\n");
  init_SPIMode();
  _displaySD->write_String("successful...\r\n");
}

cSDCardModule::~cSDCardModule()
{

}

void cSDCardModule::init_SPIMode()
{
  for(uint8_t count = 0; count < 10; count++)
  {
    send_dummyByte();
  }
  _delay_ms(1);

  send_Cmd(CMD0);
}

void cSDCardModule::send_dummyByte()
{
  _spi->transmit(0xFF);
}

void cSDCardModule::send_Cmd(uint8_t command)
{
  _spi->transmit(command);
}
