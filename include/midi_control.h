#ifndef MIDI_HELPER
#define MIDI_HELPER
#include <Arduino.h>
#include <MIDIUSB.h>
#include "config.h"

#define MAX_CC_VAL 127


enum run_modes
{
    no_interval_change,
    random_interval,
    new_interval,
};

enum Elevator{
    ascending,
    descending,
    reverse,
};


class MIDIControl  
{
private:
    uint8_t _control;
    uint16_t _interval;
    uint8_t _range[2];
    uint8_t _cc_val;
    uint32_t _lastChange;
    uint8_t _ascending;

    void setNextVal();
    uint8_t setVal(uint8_t v){(v <= 127)?v = v : v = 127;return v;}
    void controlChange();
    void debugMsg();
    void resetTimer(){_lastChange = millis();}

public:
    MIDIControl(uint8_t control_ch, uint16_t interval);
    MIDIControl(); 
    void setRange(uint8_t min, uint8_t max);
    void setCC(uint8_t cc){_control = cc;}
    void setReverse(uint8_t ascending);
    uint8_t getCCNum(){return _control;}
    
    void sendControlChange(uint8_t val);
    void setInterval(uint16_t t){_interval = t;}
    void automate();
    void automate(uint8_t mode, uint16_t t);
    void noteOn(uint8_t channel, uint8_t pitch, uint8_t velocity);
    void setup();
};

#endif