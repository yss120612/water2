#include "Speaker.h"
#include "Config.h"

Speaker::Speaker(){

};

Speaker::~Speaker(){

};

void Speaker::setup(uint8_t p){
 pin=p;
 ledcSetup(0, FREQ1, 8);
 ledcAttachPin(pin, 0);
 speaker_on=false;
}

void Speaker::activate(bool _on){
    speaker_on=_on;
    ledcWrite(0, speaker_on?128:0);
}
void Speaker::processSpeaker(unsigned long ms){
if (!speaker_on) return;
if (last_check>ms) {last_check=ms;return;}
if (ms-last_check<period) return;
last_check=ms;
ledcWriteTone(0,ledcReadFreq(0)<FREQ2-1.0?FREQ2:FREQ1);
}