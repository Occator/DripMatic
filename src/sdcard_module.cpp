#include "sdcard_module.h"
#include <util/delay.h>

cSDCardModule::cSDCardModule(cUART *uartComm, cIOPin *chipSelect,  cSPIModule * csDevice)
: _uartSD(uartComm), _csPin(chipSelect), _spi(csDevice)
{
  _uartSD->write_String("init SD-Card in SPI-mode...\r\n");
  init_SPIMode();
  _uartSD->write_String("successful...\r\n");
}

cSDCardModule::~cSDCardModule()
{

}

void cSDCardModule::init_SPIMode()
{
  _csPin->set_Pin(1);
  for(uint8_t count = 0; count < 10; count++)
  {
    send_dummyByte();
  }

  _delay_ms(1);

  send_Cmd(CMD0);
  send_Cmd(0x00);
  send_Cmd(0x00);
  send_Cmd(0x00);
  send_Cmd(0x00);
  send_Cmd(0x95);
  send_dummyByte();
  _csPin->set_Pin(0);
}

void cSDCardModule::send_dummyByte()
{
  _spi->spi_byte(0xFF);
}

void cSDCardModule::send_Cmd(uint8_t command)
{
  _spi->spi_byte(command);
}
