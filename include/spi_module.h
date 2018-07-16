#ifndef SPI_MODULE_H
#define SPI_MODULE_H

#include <avr/io.h>
#include <util/delay.h>

class cSPIModule
{
public:
  cSPIModule();
  ~cSPIModule();

  uint8_t transmit(uint8_t data);
  void transmit(const char * string);
  uint8_t receive();
private:
  void init_Master();
};
#endif
