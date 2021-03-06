#ifndef UART_MODULE_H
#define UART_MODULE_H
#include <stdio.h>
#include <avr/io.h>
#include <stdint.h>

#define F_CPU 16000000UL
#define BAUD 9600UL

class cUART{
public:
	static cUART& getInstance();

	void init();
	uint8_t read();
	void write_Char(char data);
	void write_String(const char* string);
	void write_Int(int data);
private:
	cUART();
	cUART(cUART const &);  // copy constructor
	cUART& operator=(cUART const&);  // assignment operator
	~cUART();

};

#endif
