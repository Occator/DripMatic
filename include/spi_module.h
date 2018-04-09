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
  
  uint8_t transmit(uint8_t data);
  void transmit(const char * string);
  uint8_t receive();
private:
  cIOPin *_csSPI;
  void init_master();
};
#endif
