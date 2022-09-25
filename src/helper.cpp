#include "helper.h"

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
  #if MIDI_MSG ==1
  midiMsg(pitch, velocity);
  #endif
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


void beginKaosTimer(FSM *p){
  if(!p->timer_running){
    p->kaosT = millis();
    p->timer_running^= 1;
  }
}
void printKaosCountdown(FSM *p){
  int countdonw = CHAOS_THRESHOLD_MS + p->kaosT - millis();
  Serial.print("Time to change: ");
  Serial.println(countdonw);
}
bool checkKaosTimer(FSM *p){
  if(millis()- p->kaosT > CHAOS_THRESHOLD_MS){
    return 1;
  }
  if(p->timer_running){
    #if CHAOS_TIMER_MSG ==true
      printKaosCountdown();
    #endif
    
  }
  return 0;
}
void resetKaosTimer(FSM *p){
  if(p->timer_running)p->timer_running = !p->timer_running;
}