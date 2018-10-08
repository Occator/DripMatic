#include "spi_module.h"
#include "uart_module.h"

cSPIModule::cSPIModule()
{
  init_Master();
  cUART::getInstance().write_String("SPI init() called\r\n");
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
  SPDR = 0xFF;
  while(! (SPSR & (1 << SPIF) ) );
  uint8_t data = SPDR;
  return ( data );
}

void cSPIModule::init_Master()
{
  DDRB = (1 << DDB3) | (1 << DDB5) | (1 << DDB2);
  PORTB |= (1 << PB2);
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
}
