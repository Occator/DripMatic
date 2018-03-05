#include "spi_module.h"

cSPIModule::cSPIModule()
{
  master_init();
}

cSPIModule::~cSPIModule(){}

void cSPIModule::master_init()
{
  DDRB = (1 << PB3) | (1 << PB5);
  SPCR0 = (1 << SPE0) | (1 << MSTR0) | (1 << SPR00);
}
