// Log.h
#ifndef _SPEAKER_h
#define _SPEAKER_h
#include "arduino.h"

class Speaker
{
public:
	Speaker();
	~Speaker();
	void setup(uint8_t p);
	void processSpeaker(unsigned long ms);
	void activate(bool _on);
private:
	uint16_t pin;
	bool speaker_on;
	unsigned long last_check;
	const uint16_t period=500;
};
#endif
