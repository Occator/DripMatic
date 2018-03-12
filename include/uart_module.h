#ifndef UART_MODULE_H
#define UART_MODULE_H
#include <stdio.h>
#include <avr/io.h>
#include <stdint.h>

#define F_CPU 16000000UL
#define BAUD 9600UL

class cUART{
public:
	cUART();
	~cUART();
	void init_UART(uint16_t ubbr0Value);
	uint8_t read_UART();
	void write_UART_Char(char data);
	void write_UART_String(char* string);
	void write_UART_Int(int data);
private:
};

#endif
