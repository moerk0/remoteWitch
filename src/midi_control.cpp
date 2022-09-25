#include "midi_control.h"
#define END_OF_TIMER }

MIDIControl::MIDIControl(byte control_ch, uint16_t interval):_control(control_ch),_interval(interval),_ascending(true)
{
    _lastChange = millis();
    _range[0] = 0;
    _range[1] = 127;
}

MIDIControl::MIDIControl():_control(0),_interval(10),_ascending(ascending){
    _lastChange = millis();
    _range[0] = 0;
    _range[1] = 127;
}

void MIDIControl::controlChange(){
    midiEventPacket_t event = {0x0B, 0xB0 | 0, _control, _cc_val};
  MidiUSB.sendMIDI(event);
}

void MIDIControl::setReverse(byte b){
    if(b == reverse){
        _ascending = false;
        _cc_val = MAX_CC_VAL;
    }
    else {;}
}   





void MIDIControl::debugMsg(){
      Serial.print("Control: ");
      Serial.print(_control);
      Serial.print(" | Value: ");
      Serial.print(_cc_val);
      Serial.print(" | Interval: ");
      Serial.print(_interval);
      Serial.print(" | realDiff: ");
      Serial.print(millis() - _lastChange);
      Serial.println();

}

void MIDIControl::setNextVal(){
    
    if(_ascending){
        if(_cc_val <= _range[1])_cc_val++;
        else _ascending = false;
    }
    else{
        if(_cc_val > _range[0])_cc_val--;
        else _ascending = true;

    }
    
}

void MIDIControl::setRange(byte min, byte max){
    _range[0] = (min>=0)?min: 0;
    _range[1] = (max<=127)?max: 127;
}

void MIDIControl::automate(){
    if (millis()- _lastChange > _interval){
        controlChange();
        debugMsg();
        MidiUSB.flush();
        setNextVal();
        resetTimer();
        
    END_OF_TIMER
}

/*
@param modes can be either random change or fixed change
@param if random t is max Interval, if not t is fixed value
*/
void MIDIControl::automate(byte mode, uint16_t t){
    if (millis()- _lastChange > _interval){
    switch (mode)
    {
    case random_interval:
        setInterval(random(10,t));
        break;
    case new_interval:
    setInterval(t);
        break;
    case no_interval_change:
    setInterval(t);
    break;
    }

    controlChange();
    debugMsg();
    MidiUSB.flush();
    setNextVal();
    resetTimer();
    
    END_OF_TIMER
}

void MIDIControl::sendControlChange(byte v){
    if (millis()- _lastChange > _interval){
    _cc_val = v;
    controlChange();
    #if MIDI_MSG == true
        debugMsg();
    #endif
    MidiUSB.flush();
    resetTimer();
    END_OF_TIMER
}

void MIDIControl::noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
}