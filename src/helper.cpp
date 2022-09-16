#include "helper.h"

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
  midiMsg(pitch, velocity);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

byte mapSonarVal(uint64_t inp){
  byte out = map(inp, 0, MAX_DIST,127,0);
  return out;
}


void midiMsg(uint8_t note, uint16_t val){
  Serial.print("Note: ");
  Serial.print(note);
  Serial.print(" | val: ");
  Serial.print(val);
  Serial.println();

}

void sonarMsg(uint16_t val){
  Serial.print("sonar.ping_result: ");
  Serial.print(val);
  Serial.print(" | mapSonarVal: ");
  Serial.print(mapSonarVal(val));
  Serial.println();
  
}