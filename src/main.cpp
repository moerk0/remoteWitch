#include <Arduino.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();  // Sender is connected to Pin #11

void setup() {
  mySwitch.enableTransmit(2);
  mySwitch.setProtocol(12);
}

void loop() {
   mySwitch.switchOn("11111", "11111");        // Switch 1st socket on
    delay(1000);
mySwitch.switchOff("11111", "11111");
    delay(1000);
}