#include "midi_helper.h"

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
  Serial.print("channel: ");
  Serial.print(channel);
  Serial.print(" | control: ");
  Serial.print(control);
  Serial.print(" | Value: ");
  Serial.println(value);
}

uint8_t currentMidiControl(uint8_t min_val, uint8_t max_val){
    (min_val < 0)? min_val= 0 : min_val = min_val;
    (max_val > 127)? max_val= 127 : max_val = max_val;

    static int idx;
    static bool ascending;
    if(ascending){
        if(idx < max_val)return idx++;
        else{
            ascending = !ascending;
            return idx--;
        }
    }
    else{
        if(idx > min_val)return idx --;
        else{
            ascending = !ascending;
            return idx++;
        }
    }
};