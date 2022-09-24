#ifndef FILTER_H
#define FILTER_H
#include "midi_control.h"
#define FILTER_CC_REQUIRED 3

enum Parameter{
    freqency,
    gain,
    bandwith,
};


class Filter
{
private:
    MIDIControl _param[FILTER_CC_REQUIRED];
public:
    Filter(byte startCC);
    void change(byte,byte);
    void normalize(byte);
    void automate(byte);
    void automate(byte,byte,uint16_t);
    void automate(byte,byte,uint16_t,byte,byte);
    void printName(byte);
    //Mask function. You call it in a for loop and u set odd/even numbers to reverse
    void sinus(byte cc){_param[cc].setReverse(reverse);}
    void setRange(byte cc, byte min, byte max){_param[cc].setRange(min,max);}

    byte getOccupiedCC(){return sizeof(_param);}
};

Filter::Filter(byte startCC) 
{
    for (byte i = 0; i < FILTER_CC_REQUIRED; i++)
    {
        _param[i].setCC(startCC + i);
    }
    
}

void Filter::normalize(byte which){

    _param[which].sendControlChange(64);
}

void Filter::change(byte which, byte val){
    _param[which].setInterval(10);
    _param[which].sendControlChange(val);
}

void Filter::automate(byte which){
    _param[which].automate();
}

void Filter::automate(byte which, byte mode, uint16_t interval){
    _param[which].automate(mode,interval);
}
void Filter::automate(byte which, byte mode, uint16_t interval, byte min, byte max){
    _param[which].setRange(min,max);
    _param[which].automate(mode,interval);
}
void Filter::printName(byte which){
    switch (which)
    {
    case freqency:
        Serial.print("Frequency ||");
        break;
    case gain:
        Serial.print("   Gain ||");
        break;
    case bandwith:
        Serial.print("   bandwith ||");
        break;
    
    default:
        break;
    }
}
#endif