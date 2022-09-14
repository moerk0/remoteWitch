#ifndef MIDI_HELPER
#define MIDI_HELPER
#include <Arduino.h>
#include <MIDIUSB.h>

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
    byte _control;
    uint16_t _interval;
    byte _range[2];
    byte _cc_val;
    uint32_t _lastChange;
    byte _ascending;

    void setNextVal();
    byte setVal(byte v){(v <= 127)?v = v : v = 127;return v;}
    void controlChange();
    void debugMsg();
    void resetTimer(){_lastChange = millis();}

public:
    MIDIControl(byte control_ch, uint16_t interval);
    MIDIControl(); 
    void setRange(byte min, byte max);
    void setCC(byte cc){_control = cc;}
    void setReverse(byte ascending);
    
    void sendControlChange(byte val);
    void setInterval(uint16_t t){_interval = t;}
    void run();
    void run(byte mode, uint16_t t);
};

#endif