#ifndef SPI_MODULE_H
#define SPI_MODULE_H

#include <avr/io.h>
#include <util/delay.h>
#include "pin_io.h"
#include "uart_module.h"

class cSPIModule
{
public:
  cSPIModule(cIOPin *csDevice);
  ~cSPIModule();
  void transmit(uint8_t data);
  void transmit(const char * string);
  uint8_t spi_byte(uint8_t data);
private:
  cIOPin *_csSPI;
  void init_master();
};
#endif
