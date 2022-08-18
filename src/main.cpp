#include <Arduino.h>
#include "RCSwitch.h"
#include "Button.h"

#define RC_PIN 13
#define BUTTON_PIN 7
#define BUTTON_DEBOUNCE 300
#define ADDR_1 30
#define ADDR_2 29
#define ADDR_3 27
#define TASTE_A 1
#define TASTE_B 2

enum Plugs{
  plug0,
  plug1,
  plug2,
  plug3,
  plug4,
  plug5,
  cnt_plug,
};

Button but(BUTTON_PIN,0,BUTTON_DEBOUNCE);

RCSwitch rc[] = {
  RCSwitch(RC_PIN,ADDR_1,TASTE_A),RCSwitch(RC_PIN,ADDR_1,TASTE_B),
  RCSwitch(RC_PIN,ADDR_2,TASTE_A),RCSwitch(RC_PIN,ADDR_2,TASTE_B),
  RCSwitch(RC_PIN,ADDR_3,TASTE_A),RCSwitch(RC_PIN,ADDR_3,TASTE_B),
  };



void setup() {
 Serial.begin(9600);
 int d = 1000;
 uint16_t o = d;
 for(int i =0; i<cnt_plug; i++){
  rc[i].setDelayT(d);
  rc[i].setLastChange(o);
  o+=d;
  Serial.println(rc[i].getLastChange());
}



}

void loop() {
  but.setLogic();
  but.printLogic();

  if(but.getLogic()){
    rc[plug0].switchChange();
    delay(1000);
    rc[plug0].switchChange();
    delay(10);
    rc[plug1].switchChange();
    delay(1000);
    rc[plug1].switchChange();
    delay(10);
    rc[plug2].switchChange();
    delay(1000);
    rc[plug2].switchChange();
    delay(10);
    rc[plug3].switchChange();
    delay(1000);
    rc[plug3].switchChange();
    delay(10);
    rc[plug4].switchChange();
    delay(1000);
    rc[plug4].switchChange();
    delay(10);
    rc[plug5].switchChange();
    delay(1000);
    rc[plug5].switchChange();
    delay(10);
  }
  else{
    delay(10);
    for (int i = 0; i < cnt_plug; i++)
    {
      if(rc[i].getState())
      rc[i].switchOFF();
    }
    
  }
}
