#include <Arduino.h>
#include "RCSwitch.h"

#define RC_PIN 13
#define ADDR_1 30
#define ADDR_2 29
#define ADDR_3 27
#define TASTE_A 1
#define TASTE_B 2

RCSwitch rc[] = {
  RCSwitch(RC_PIN,ADDR_1,TASTE_A),RCSwitch(RC_PIN,ADDR_1,TASTE_B),
  RCSwitch(RC_PIN,ADDR_2,TASTE_A),RCSwitch(RC_PIN,ADDR_2,TASTE_B),
  RCSwitch(RC_PIN,ADDR_3,TASTE_A),RCSwitch(RC_PIN,ADDR_3,TASTE_B),
  };


void setup() {
 Serial.begin(115200);


}

void loop() {
  
  rc[0].switchON();
  delay(1000);
  
  
  rc[0].switchOFF();
  
  delay(1000);
}