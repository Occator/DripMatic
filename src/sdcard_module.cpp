#include "sdcard_module.h"
#include <util/delay.h>

cMicroSDModule::cMicroSDModule(cUART *uartComm, cIOPin *chipSelect, cSPIModule * csDevice)
: _uartSD(uartComm), _csPin(chipSelect), _spi(csDevice)
{
  _uartSD->write_String("init SD-Card in SPI-mode...\r\n");
  if (!_initSPIMode())
  {
    _isSuccessful = true;
    _uartSD->write_String("init successful...\r\n");
  }
  else
  {
    _isSuccessful = false;
    _uartSD->write_String("init not successful...\r\n");
  }

  // get register information
  
}

cMicroSDModule::~cMicroSDModule()
{}

uint8_t cMicroSDModule::_initSPIMode()
{
  uint8_t response;
  uint8_t retry {0};

  _csAsserted(); // cs asserted
  do
  {
    for(uint8_t i = 0; i < 10; i++)
    {
      _spi->transmit(0xFF);
    }
    //_csPin->set_Pin(1);
    response = _sendCommand(GO_IDLE_STATE, 0);
    _uartSD->write_String("reset SD card\r\n");

    retry++;
    if(retry > 15)
    {
      _uartSD->write_String("timeout... no card detected\r\n");
      return 1;
    }
  } while(response != 0x01);

  _csDeasserted();  // cs deasserted

  _spi->transmit(0xFF);
  _spi->transmit(0xFF);

  retry = 0;
  _uartSD->write_String("send HCS info and activate card init process\r\n");
  do {
    response = _sendCommand(SEND_OP_COND, 0);
    retry++;
    if(retry > 15)
    {
      return 1;
    }
    _uartSD->write_String("command failed ... retry\r\n");
  } while(response);
  _uartSD->write_String("init process successful activated\r\n");

  _sendCommand(CRC_ON_OFF, 0);
  _uartSD->write_String("CRC option off\r\n");

  _sendCommand(SET_BLOCK_LEN, 512);
  _uartSD->write_String("set block length to 512 bytes\r\n");

  return 0;
}

uint8_t cMicroSDModule::_sendCommand(uint8_t command, uint32_t argument)
{
  uint8_t response;
  uint8_t retry {0};

  _csAsserted();

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
  _csDeasserted();
  return (response);
}

void cMicroSDModule::_csAsserted()
{
  _csPin->set_Pin(0);
}

void cMicroSDModule::_csDeasserted()
{
  _csPin->set_Pin(1);
}
