#ifndef SPI_MODULE_H
#define SPI_MODULE_H

#include <avr/io.h>

class cSPIModule
{
public:
  cSPIModule();
  ~cSPIModule();
  void master_init();
  void transfer_byte(uint8_t data);
  uint8_t receive_byte();

};
#endif
