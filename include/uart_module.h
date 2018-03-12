#ifndef _class_UART_Conf_Atmega328pH_
#define _class_UART_Conf_Atmega328pH_
#include <stdio.h>
#include <avr/io.h>
#include <stdint.h>

#define F_CPU 16000000UL
#define BAUD 9600UL

class cUSART{
public:
	cUSART();
	~cUSART();
	void init_USART(uint16_t ubbr0Value);
	uint8_t read_USART();
	void write_USART_Char(char data);
	void write_USART_String(char* string);
	void write_USART_Int(int data);
private:
};

#endif