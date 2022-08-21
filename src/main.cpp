#include <Arduino.h>
#include <millisDelay.h>
#include "RCSwitch.h"
#include "Button.h"
#include "chase.h"

#define RC_PIN 13
#define BUTTON_PIN 7
#define BUTTON_DEBOUNCE 300
#define ADDR_1 30
#define ADDR_2 29
#define ADDR_3 27
#define TASTE_A 1
#define TASTE_B 2
#define DEFAULT_DELAY 2400

struct TimeManager{
  int delay_on;
  int delay_off;
};

enum Plugs{
  plug0,
  plug1,
  plug2,
  plug3,
  plug4,
  plug5,
  cnt_plug,
};

enum Buttons{
  but0
};

Button but(BUTTON_PIN,but0,BUTTON_DEBOUNCE);
ChasePLUGS handler(cnt_plug,DEFAULT_DELAY);

RCSwitch rc[] = {
  RCSwitch(RC_PIN,ADDR_1,TASTE_A),RCSwitch(RC_PIN,ADDR_1,TASTE_B),
  RCSwitch(RC_PIN,ADDR_2,TASTE_A),RCSwitch(RC_PIN,ADDR_2,TASTE_B),
  RCSwitch(RC_PIN,ADDR_3,TASTE_A),RCSwitch(RC_PIN,ADDR_3,TASTE_B),
  };

void setup() {
 Serial.begin(115200);
//  int o = abs(DEFAULT_DELAY / cnt_plug);
//  delay(1000);
//  for (int i = 0; i < cnt_plug; i++)
//  {
//   rc[i].setdelayT(o,o*i);
//  }
 
}



int tries = 0;

void loop() {
  but.setLogic();
  but.printLogic();
  int t2c = handler.time2change(0);
  if(but.getLogic()){
    
    tries = 0;  

    if(t2c>=0){
      // Serial.print("--------Begin Circle-------- \nCurrent Plug:");
      // Serial.println(t2c);
      rc[t2c].switchON();
      delay(10);
      int prev_p = handler.previousPlug(1);
      rc[prev_p].switchOFF();
      // Serial.print("Previous PLug: ");
      // Serial.println(prev_p);
      // Serial.println("--------End of Circle--------");
    }


  }
  else{
    if (tries<=10)
    {
    handler.setAdvanceTime(100);
    }
    else{handler.setAdvanceTime(DEFAULT_DELAY);}
    
    if (t2c>=0)
    {
      rc[t2c].switchOFF();
      tries++;
      
    }
      

    
  }
}

