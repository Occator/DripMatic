#ifndef SPI_MODULE_H
#define SPI_MODULE_H

#include <avr/io.h>

class cSPIModule
{
public:
  cSPIModule();
  ~cSPIModule();

private:
  void master_init();
  void master_transmit(uint8_t data);
  uint8_t master_receive();

};
#endif
