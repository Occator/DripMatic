// tested and worked at 2017/09/28 22:06
#ifndef CLASS_TWI_H
#define CLASS_TWI_H

#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>

#define F_CPU 16000000UL
#define S_CLK 400000UL
#define TWBR_VALUE (((F_CPU / S_CLK) - 16) / 2)

// status-codes for Master Transmit Mode (MT-Mode)
#define TWI_START 0x08U
#define SLA_W_ACK 0x18U
#define SLA_W_NACK 0x20U
#define DATA_ACK 0x28U
#define DATA_NACK 0x30U

class cTWI{
public:
	cTWI(uint8_t devAddress);
	~cTWI();

	void init();
	void start();
	void stop();
	void write(uint8_t data);
	void transmit(uint8_t data);
	bool get_StartFlag();
	bool get_SlaAckFlag();
	bool get_DataAckFlag();
private:
	uint8_t bcd_To_Dec(uint8_t bcdByte);
	uint8_t dec_To_BCD(uint8_t decByte);
	uint32_t bin_To_Hex(uint32_t binByte);
	bool _startFlag = false;
	bool _slaAckFlag = false;
	bool _dataAckFlag = false;
	uint8_t _devAddr;
};

#endif