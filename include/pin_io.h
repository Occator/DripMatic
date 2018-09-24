#ifndef CLASS_IO_PIN_H
#define CLASS_IO_PIN_H

#include <avr/io.h>

class cIOPin
{
public:
	enum eDirection{
		input = 0, output = 1
	};

	cIOPin();
	cIOPin( volatile uint8_t *port, const uint8_t bit, eDirection direction);
	~cIOPin();

	void SetPin(bool value);
	uint8_t GetPin();
	void TogglePin();
	void ToggleDirection();
	void SetDirection(bool direction);
private:

	const uint8_t _defaultInit {0};
	volatile uint8_t *_port;
	const uint8_t _bitVal;
	eDirection _dir;
};

#endif
