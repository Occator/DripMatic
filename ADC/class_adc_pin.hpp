#ifndef CLASS_ADC_PIN_H
#define CLASS_ADC_PIN_H

class cADCPin {
public:
	cADCPin(uint8_t ch);
	~cADCPin();

	void init();
	uint16_t read();
private:
	uint8_t _channel;
	// keep looking for further implementation of initializing member variables
};

#endif