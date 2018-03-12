#include "class_uart_conf_atmega328p.hpp"
#include <stdlib.h>

cUSART::cUSART(){
	uint16_t ubbr0Value = (F_CPU / (16 * BAUD) - 1);
	init_USART(ubbr0Value);
}

cUSART::~cUSART(){}

void cUSART::init_USART(uint16_t ubbr0Value){
	// set baud rate
	UBRR0L = ubbr0Value;
	UBRR0H = (ubbr0Value >> 8);

	// set frame format: asynchronous mode, no parity, 1 stop bit, character size 8 bit
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);

	// enable Receiver and Transmitter
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
}

uint8_t cUSART::read_USART(){
	// wait until data is available
	while(!((1 << RXEN0) | (1 << TXEN0))){}
	return UDR0;
}
void cUSART::write_USART_Char(char data){
	// wait until transmitter is ready
	while(!(UCSR0A & (1 << UDRE0))){}
	UDR0 = data;
}
void cUSART::write_USART_Int(int data){
	char buffer[5];
	itoa(data, buffer, 10);
	write_USART_String(buffer);
}
void cUSART::write_USART_String(char* string){
	while(*string != '\0'){
			write_USART_Char(*string);
			string++;
		}
}