#include "sdcard_module.h"
#include "uart_module.h"
#include <util/delay.h>

cMicroSDModule::cMicroSDModule(cIOPin *chipSelect, cSPIModule * csDevice)
: _csPin(chipSelect), _spi(csDevice)
{
  cUART::getInstance()->write_String("**init SD-Card in SPI-mode...\r\n");
  if (!_initSPIMode())
  {
    _isSuccessful = true;
  }
  else
  {
    _isSuccessful = false;
  }
}

cMicroSDModule::~cMicroSDModule()
{}


uint8_t cMicroSDModule::readSingleBlock(uint8_t *buffer, uint32_t startBlock)
{
  cUART::getInstance()->write_String("**enter readSingleBlock\r\n");
    uint8_t response;
    response  = sendCommand(READ_SINGLE_BLOCK, ( (uint32_t)startBlock) << 9);
    if(response != 0x00)
    {
      return response;
    }

    _csAsserted();

    for(uint8_t retry = 0; retry < 50; retry++)
    {
      response = _spi->receive();
      if(response == SD_START_TOKEN)
      {
        retry = 0;
        break;
      }
    }

    for(uint16_t i = 0; i < BLOCK_LENGTH; i++)
    {
      *buffer++ = _spi->receive();
    }

    _spi->receive(); // discard incomming CRC - 16-bits
    _spi->receive();

    _spi->receive();
    _csDeasserted();

  cUART::getInstance()->write_String("**exit readSingleBlock\r\n");
  return 0;
}

uint8_t cMicroSDModule::writeSingeBlock(uint8_t *buffer, uint32_t startBlock)
{
  cUART::getInstance()->write_String("**enter writeSingleBlock\r\n");
    uint8_t response;
    response = sendCommand(WRITE_SINGLE_BLOCK, ( (uint32_t)startBlock) << 9);

    if(response != 0x00)
    {
      return response;
    }

    _csAsserted();

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

    while( !_spi->receive() );

    _csDeasserted();
    _spi->transmit(0xFF);
    _csAsserted();

    while( !_spi->receive() );

    _csDeasserted();
  cUART::getInstance()->write_String("**exit writeSingleBlock\r\n");
  return 0;
}

uint8_t cMicroSDModule::_initSPIMode()
{
  uint8_t response, sdVersion;
  uint8_t retry {0};

  for(uint8_t i = 0; i < 10; i++)
  {
    _spi->transmit(0xFF);
  }

  _csAsserted();
  do
  {
    response = sendCommand(GO_IDLE_STATE, 0);
    retry++;
    if(retry > 15)
    {
      cUART::getInstance()->write_String("**timeout... no card detected\r\n");
      return 1;
    }
  } while(response != 0x01);

  _csDeasserted();
  _spi->transmit(0xFF);
  _spi->transmit(0xFF);

  retry = 0;

  sdVersion = 2;

  do
  {
    response = sendCommand(SEND_IF_COND, 0x000001AA);
    retry++;
    if(retry > 15)
    {
      sdVersion = 1;
      _cardType = 1;
      break;
    }
  } while(response != 0x01);


  retry = 0;

  for(uint8_t j = 0; j < 10; j++)
  {
    response = _sendAppCmd();
    if(response == 0x00)
    {
      break;
    }
  }

  _sdhcFlag = 0;

  if(sdVersion == 2)
  {
    do
    {
      response = sendCommand(READ_OCR, 0);
      retry++;
      if(retry > 20)
      {
        _cardType = 0;
        break;
      }
    }while(response != 0x00);

    if(_sdhcFlag == 1)
    {
      _cardType = 2;
    }
    else
    {
      _cardType = 3;
    }
  }

  sendCommand(CRC_ON_OFF, 0);
  sendCommand(SET_BLOCK_LEN, BLOCK_LENGTH);

  return 0;
}

uint8_t cMicroSDModule::sendCommand(uint8_t command, uint32_t argument)
{
  uint8_t response, status;
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

  if(response == 0x00 && command == READ_OCR)
  {
    status = _spi->receive() & 0x40;  // first byte of OCR register (bits 31:24)
    if(status == 0x40)
    {
      _sdhcFlag = 1;
    }
    else
    {
      _sdhcFlag = 0;
    }
    _spi->receive();  // three remaining bytes of OCR register are discarded
    _spi->receive();
    _spi->receive();
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

  for(uint8_t i = 0; i < 50; i++)
  {
    response = sendCommand(APP_CMD, 0);
    response = sendCommand(SD_SEND_OP_COND, 0x40000000);
    if(response == 0x00)
    {
      break;
    }
  }
  return response;
}
