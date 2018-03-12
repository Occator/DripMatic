#include "uart_module.h"
#include <stdlib.h>

cUART::cUART(){
	uint16_t ubbr0Value = (F_CPU / (16 * BAUD) - 1);
	init(ubbr0Value);
}

cUART::~cUART(){}

void cUART::init(uint16_t ubbr0Value){
	// set baud rate
	UBRR0L = ubbr0Value;
	UBRR0H = (ubbr0Value >> 8);

	// set frame format: asynchronous mode, no parity, 1 stop bit, character size 8 bit
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);

	// enable Receiver and Transmitter
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
}

uint8_t cUART::read(){
	// wait until data is available
	while(!((1 << RXEN0) | (1 << TXEN0))){}
	return UDR0;
}
void cUART::write_Char(char data){
	// wait until transmitter is ready
	while(!(UCSR0A & (1 << UDRE0))){}
	UDR0 = data;
}
void cUART::write_Int(int data){
	char buffer[5];
	itoa(data, buffer, 10);
	write_String(buffer);
}
void cUART::write_String(const char* string){
	while(*string != '\0'){
			write_Char(*string);
			string++;
		}
}
