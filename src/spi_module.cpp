#include "spi_module.h"

cSPIModule::cSPIModule()
{
  master_init();
}

cSPIModule::~cSPIModule(){}

void cSPIModule::master_init()
{
  DDRB = (1 << PB3) | (1 << PB5) | (1 << PB2);
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void cSPIModule::send_byte(uint8_t data)
{
  SPDR = data;
  while(! (SPSR & (1 << SPIF) ) );
}

uint8_t cSPIModule::receive_byte()
{
  return 0;
}
