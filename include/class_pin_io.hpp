#ifndef CLASS_IO_PIN_H
#define CLASS_IO_PIN_H

#include <avr/io.h>

class cConfigIOPin{
public:
	virtual void set_Pin(bool value) = 0;
	virtual uint8_t get_Pin() = 0;
	virtual void toggle_Pin() = 0;
	virtual void toggle_Direction() = 0;
	virtual void set_Direction(bool direction) = 0;
};

class cIOPin : public cConfigIOPin{
public:
	enum eDirection{
		input = 0, output = 1
	};

	cIOPin( volatile uint8_t *port, const uint8_t bit, eDirection direction);
	~cIOPin();

	void set_Pin(bool value);
	uint8_t get_Pin();
	void toggle_Pin();
	void toggle_Direction();
	void set_Direction(bool direction);
private:

	volatile uint8_t *_port;
	const uint8_t _bitVal;
	eDirection _dir;
};

#endif
