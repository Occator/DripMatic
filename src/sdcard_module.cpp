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

  for(uint8_t j = 0; j < 10; j++)
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

  sendCommand(SET_BLOCK_LEN, BLOCK_LENGTH);

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

uint8_t cMicroSDModule::readSingleBlock(uint8_t *buffer, uint32_t startBlock)
{
  _csAsserted();
  uint8_t response;
  response  = sendCommand(READ_SINGLE_BLOCK, ( (uint32_t)startBlock) << 9);

  while(response != 0x00)
  {
    response = _spi->receive();
  }

  do
  {
    response = _spi->receive();
  }  while(response != 0xFE);

  for(uint16_t i = 0; i < BLOCK_LENGTH; i++)
  {
    *buffer++ = _spi->receive();
  }

  _spi->receive(); // discard incomming CRC - 16-bits
  _spi->receive();
  _csDeasserted();

  _spi->receive();

  return response;
}

uint8_t cMicroSDModule::writeSingeBlock(uint8_t *buffer, uint32_t startBlock)
{
  _csAsserted();
  uint8_t response;
  response = sendCommand(WRITE_SINGLE_BLOCK, ( (uint32_t)startBlock) << 9);

  while(response != 0x00)
  {
    response = _spi->receive();
  }

  for(uint8_t k = 0; k < 3; k++)
  {
    _spi->receive();
  }

  _spi->transmit(0xFE);

  for(uint16_t i = 0; i < 512; i++)
  {
    _spi->transmit(*buffer++);
  }
  _spi->transmit(0xFF);
  _spi->transmit(0xFF);

  response = _spi->transmit(0xFF);

  if( (response & 0x1F) != 0x05)
  {
    _csDeasserted();
    return response;
  }

  for(uint8_t j = 0; j < 20; j++)
  {
    response = _spi->transmit(0xFF);
  }

  do
  {
      response = _spi->receive();
  } while(response != 0x00);

  for(uint8_t j = 0; j < 20; j++)
  {
    response = _spi->transmit(0xFF);
  }

  _spi->receive();
  _csDeasserted();

  return response;

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
