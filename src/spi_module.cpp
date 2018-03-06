#include "spi_module.h"

cSPIModule::cSPIModule()
{
  master_init();
}

cSPIModule::~cSPIModule(){}

void cSPIModule::master_init()
{
  DDRB = (1 << PB3) | (1 << PB5) | (1 << PB2);
  DDRB &= ~(1 << PB4);
  PORTB |= (1 << PB2);
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);  // | (1 << CPOL) | (1 << CPHA)
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
