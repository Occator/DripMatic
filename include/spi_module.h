#ifndef SPI_MODULE_H
#define SPI_MODULE_H

#include <avr/io.h>
#include <util/delay.h>
#include "pin_io.h"

class cSPIModule
{
public:
  cSPIModule(cIOPin *csDevice);
  ~cSPIModule();
  void transmit(uint8_t data);
  void transmit(const char * string);
private:
  cIOPin *_csSPI;
  void master_init();
  void send_byte(uint8_t data);
  uint8_t receive_byte();

};
#endif
