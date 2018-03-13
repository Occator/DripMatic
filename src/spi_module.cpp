#include "spi_module.h"

cSPIModule::cSPIModule(cIOPin *csDevice) : _csSPI(csDevice)
{
  init_master();
}

cSPIModule::~cSPIModule(){}

void cSPIModule::transmit(uint8_t data)
{
  _csSPI->set_Pin(1);
  _delay_ms(1);
  send_byte(data);
  _csSPI->set_Pin(0);
}
void cSPIModule::transmit(const char * string)
{
    while(*string != '\0')
    {
      send_byte(*string);
      string++;
    }
}

void cSPIModule::init_master()
{
  DDRB = (1 << DDB3) | (1 << DDB5) | (1 << DDB2);
  DDRB &= ~(1 << DDB4);
  _csSPI->set_Pin(0);
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
}

void cSPIModule::send_byte(uint8_t data)
{
  SPDR = data;
  while(! (SPSR & (1 << SPIF) ) );
}

uint8_t cSPIModule::receive_byte()
{
  SPDR = 0xFF; // dummy send value to read out receive-register
  while(! (SPSR & (1 << SPIF) ) );
  return (SPDR);
}
