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
{

}

void cMicroSDModule::init_SPIMode()
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

void cMicroSDModule::send_dummyByte()
{
  _spi->spi_byte(0xFF);
}

uint8_t cMicroSDModule::send_Cmd(uint8_t command)
{
  _spi->spi_byte(command);
}
