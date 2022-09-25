#include <Arduino.h>
#include <MIDIUSB.h>
#include "config.h"
#include "globals.h"

void noteOn(byte channel, byte pitch, byte velocity);
void noteOff(byte channel, byte pitch, byte velocity);
void controlChange(byte channel, byte control, byte value);
uint8_t currentMidiControl(uint8_t min_val, uint8_t max_val);
byte mapSonarVal(uint64_t inp);
void sonarMsg(uint16_t);
void midiMsg(uint8_t note, uint16_t val);


void beginKaosTimer    (FSM *p);
void printKaosCountdown(FSM *p);
bool checkKaosTimer    (FSM *p);
void resetKaosTimer    (FSM *p);