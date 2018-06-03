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

}

cMicroSDModule::~cMicroSDModule()
{}

uint8_t cMicroSDModule::_initSPIMode()
{
  uint8_t response;
  uint8_t retry {0};

  _csAsserted();
  for(uint8_t i = 0; i < 10; i++)
  {
    _spi->transmit(0xFF);
  }

  _csDeasserted();

  _delay_ms(2);

  _csAsserted();
  do
  {
    response = sendCommand(GO_IDLE_STATE, 0);

    retry++;
    if(retry > 15)
    {
      _uartSD->write_String("timeout... no card detected\r\n");
      return 1;
    }
  } while(response != 0x01);
  _spi->transmit(0xFF);

  _csDeasserted();
  _spi->transmit(0xFF);

  retry = 0;
  _csAsserted();
  do
  {
    response = sendCommand(SEND_IF_COND, 0x000001AA);
    retry++;
    if(retry > 15)
    {
      return 1;
    }
  } while(response != 0x01);
  _spi->transmit(0xFF);

  retry = 0;
  _csDeasserted();
  _spi->transmit(0xFF);

  for(uint8_t j = 0; j < 3; j++)
  {
    _csAsserted();
    _delay_ms(1);

    response = _sendAppCmd();

    _spi->transmit(0xFF);
    _delay_ms(1);
    _csDeasserted();
  }

  _csAsserted();
  _delay_ms(1);

  _sendOCRCmd();

  _spi->transmit(0xFF);
  _delay_ms(1);
  _csDeasserted();

  return 0;
}

uint8_t cMicroSDModule::sendCommand(uint8_t command, uint32_t argument)
{
  uint8_t response;
  uint8_t retry {0};

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
  else if(command == GO_IDLE_STATE)
  {
    _spi->transmit(0x95);
  }
  else
  {
    _spi->transmit(0xFF);
  }

  while( (response = _spi->receive()) == 0xFF)
  {
    if(retry++ > 20)
    {
      break;
    }
  }

  _spi->receive();
  return (response);
}

uint8_t cMicroSDModule::readSingleBlock(uint32_t startBlock)
{
  uint8_t response;
  uint8_t retry {0};
  _csAsserted();
  do
  {
    response = sendCommand(READ_SINGLE_BLOCK, startBlock << 9);
    retry++;
    if(retry > 15)
    {
      return 1;
    }
  } while(response != 0x00);

  _uartSD->write_String("Response 0x00\r\n");

  retry = 0;
  while( (response = _spi->receive()) != 0xFE)
  {
    _uartSD->write_String("waiting for response 0xFE\r\n");
    if(retry++ > 20)
    {
      break;
    }
  }
  _uartSD->write_String("Response 0xFE\r\n");

  for(uint8_t i = 0; i < BLOCK_LENGTH; i++)
  {
    _rwBuffer[i] = _spi->receive();
  }

  _spi->receive(); // discard incomming CRC - 16-bits
  _spi->receive();
  _csDeasserted();

  _spi->receive();
  if(response != 0xFE)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}



void cMicroSDModule::_csAsserted()
{
  _csPin->set_Pin(0);
}

void cMicroSDModule::_csDeasserted()
{
  _csPin->set_Pin(1);
}

uint8_t cMicroSDModule::_sendAppCmd()
{
  uint8_t response;
  uint8_t retry {0};

  sendCommand(APP_CMD, 0);

  do
  {
    response = _spi->receive();
    retry++;
    if(retry > 5)
    {
      break;
    }
  } while(response != 0x01);

  _spi->transmit(0xFF);
  _csDeasserted();
  _spi->transmit(0xFF);

  retry = 0;
  _delay_ms(1);

  _csAsserted();
  _delay_ms(1);

  sendCommand(SD_SEND_OP_COND, 0);

  do
  {
    response = _spi->receive();
    retry++;
    if(retry > 5)
    {
      break;
    }
  } while(response != 0x00);

  _spi->transmit(0xFF);
  _csDeasserted();
  _spi->transmit(0xFF);

  return response;
}

uint8_t cMicroSDModule::_sendOCRCmd()
{
  uint8_t response;

  _spi->transmit(0xFF);
  response = sendCommand(READ_OCR, 0);

  if(response != 0x00)
  {
    for(uint8_t i = 1; i < 5; i++)
    {
      _ocrRegister[i] = _spi->receive();
    }
    _spi->transmit(0xFF);
  }

  for(uint8_t i = 0; i < 5; i++)
  {
    _ocrRegister[i] = _spi->receive();
  }
  _spi->transmit(0xFF);

  return response;
}
