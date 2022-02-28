#include <Arduino.h>
#include "RCSwitch.h"

RCSwitch rc(4);


void setup() {
 Serial.begin(115200);

}

void loop() {
  rc.switchON(30,1);
  delay(1000);
  rc.switchOFF(30,1);
  delay(1000);
 
}