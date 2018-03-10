#ifndef SPI_MODULE_H
#define SPI_MODULE_H

#include <avr/io.h>
#include <util/delay.h>

class cSPIModule
{
public:
  cSPIModule();
  ~cSPIModule();
  void master_init();
  void send_byte(uint8_t data);
  uint8_t receive_byte();

};
#endif
