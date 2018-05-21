#include "sdcard_module.h"
#include <util/delay.h>

cMicroSDModule::cMicroSDModule(cUART *uartComm, cIOPin *chipSelect, cSPIModule * csDevice)
: _uartSD(uartComm), _csPin(chipSelect), _spi(csDevice)
{
  _uartSD->write_String("init SD-Card in SPI-mode...\r\n");
  if (!initSPIMode())
  {
    _isSuccessful = false;
    _uartSD->write_String("not successful...\r\n");
  }
  _isSuccessful = true;
  _uartSD->write_String("successful...\r\n");
}

cMicroSDModule::~cMicroSDModule()
{}

uint8_t cMicroSDModule::initSPIMode()
{
  uint8_t response;
  uint8_t retry {0};

  csAsserted(); // cs asserted
  do
  {
    for(uint8_t i = 0; i < 10; i++)
    {
      _spi->transmit(0xFF);
    }
    //_csPin->set_Pin(1);
    response = sendCommand(GO_IDLE_STATE, 0);

    retry++;
    if(retry > 15)
    {
      _uartSD->write_String("timeout... no card detected\r\n");
      return 1;
    }
  } while(response != 0x01);

  csDeasserted();  // cs deasserted

  _spi->transmit(0xFF);
  _spi->transmit(0xFF);

  retry = 0;
  do {
    response = sendCommand(SEND_OP_COND, 0);
    retry++;
    if(retry > 15)
    {
      return 1;
    }
  } while(response);

  sendCommand(CRC_ON_OFF, 0);
  sendCommand(SET_BLOCK_LEN, 512);

  return 0;
}

uint8_t cMicroSDModule::sendCommand(uint8_t command, uint32_t argument)
{
  uint8_t response;
  uint8_t retry {0};

  csAsserted();

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

  while( (response = _spi->receive()) == 0xFF)
  {
    if(retry++ > 20)
    {
      break;
    }
  }

  _spi->receive();
  csDeasserted();
  return (response);
}

void cMicroSDModule::csAsserted()
{
  _csPin->set_Pin(0);
}

void cMicroSDModule::csDeasserted()
{
  _csPin->set_Pin(1);
}
