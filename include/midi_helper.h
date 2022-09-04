#include <Arduino.h>
#include <MIDIUSB.h>

void noteOn(byte channel, byte pitch, byte velocity);
void noteOff(byte channel, byte pitch, byte velocity);
void controlChange(byte channel, byte control, byte value);
uint8_t currentMidiControl(uint8_t min_val, uint8_t max_val);