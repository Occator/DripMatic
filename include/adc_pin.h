#ifndef ADC_PIN_H
#define ADC_PIN_H

class cADCPin {
public:
	cADCPin(uint8_t ch);
	~cADCPin();
	uint16_t read();
private:
	void init();
	uint8_t _channel;
};

#endif
