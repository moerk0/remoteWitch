#include "midi_control.h"
#define END_OF_TIMER }

MIDIControl::MIDIControl(uint8_t control_ch, uint16_t interval):_control(control_ch),_interval(interval),_ascending(true)
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

void MIDIControl::setReverse(uint8_t b){
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

void MIDIControl::setRange(uint8_t min, uint8_t max){
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
void MIDIControl::automate(uint8_t mode, uint16_t t){
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
    #if MIDI_MSG ==1
    debugMsg();
    #endif
    MidiUSB.flush();
    setNextVal();
    resetTimer();
    
    END_OF_TIMER
}

void MIDIControl::sendControlChange(uint8_t v){
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

void MIDIControl::noteOn(uint8_t channel, uint8_t pitch, uint8_t velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
}

void MIDIControl::setup(){
    setInterval(100);
    sendControlChange(MAX_CC_VAL);
}