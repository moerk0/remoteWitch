#include <Arduino.h>
#include "RCSwitch.h"

RCSwitch rc(2);


void setup() {
 Serial.begin(115200);

rc.setRepetitions(10);
}

void loop() {
  // rc.switchON(30,1);
  // delay(1000);
  // rc.switchON(30,2);
  // delay(1000);
  rc.switchON(30,4);
  delay(1000);
  // rc.switchOFF(30,1);
  // delay(1000);
  // rc.switchOFF(30,2);
  // delay(1000);
  rc.switchOFF(30,4);
  delay(1000);
  
 
}