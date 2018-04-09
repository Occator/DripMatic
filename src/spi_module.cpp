#include "spi_module.h"

cSPIModule::cSPIModule()
{
  init_master();
}

cSPIModule::~cSPIModule(){}

uint8_t cSPIModule::transmit(uint8_t data)
{
  SPDR = data;
  while(! (SPSR & (1 << SPIF) ) );
  data = SPDR;
  return (data);
}
void cSPIModule::transmit(const char * string)
{
    while(*string != '\0')
    {
      transmit(*string);
      string++;
    }
}

uint8_t cSPIModule::receive()
{
  return ( transmit(0xFF) );
}

void cSPIModule::init_master()
{
  DDRB = (1 << DDB3) | (1 << DDB5) | (1 << DDB2);
  DDRB &= ~(1 << DDB4);
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
}
