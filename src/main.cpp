#include <Arduino.h>
#include <millisDelay.h>
#include <RGB_LED.h>

#include "RCSwitch.h"
#include "Button.h"
#include "chase.h"

#define RC_PIN 13
#define BUTTON_PIN 7
#define RED_PIN 6
#define GREEN_PIN 5
#define BLUE_PIN 9

#define DEFAULT_DELAY 2400

struct TimeManager{
  int delay_on;
  int delay_off;
};
enum RunStates{
  standby,
  running,
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
RGB_LED led(RED_PIN, GREEN_PIN,BLUE_PIN);

RCSwitch rc[] = {
  RCSwitch(RC_PIN,ADDR_1,TASTE_A),RCSwitch(RC_PIN,ADDR_1,TASTE_B),
  RCSwitch(RC_PIN,ADDR_2,TASTE_A),RCSwitch(RC_PIN,ADDR_2,TASTE_B),
  RCSwitch(RC_PIN,ADDR_3,TASTE_A),RCSwitch(RC_PIN,ADDR_3,TASTE_B),
  };

void setup() {
 Serial.begin(115200);
 led.setFunction(Fade);
}


void statemamschine(uint8_t state){
  static int turnOffTries;
  int nextPlug = handler.getNextPlug(0);
  switch (state)
  {
  case standby:
    if (turnOffTries<=10)handler.setAdvanceTime(100);
    else handler.setAdvanceTime(10000);
      
    if (nextPlug>=0)
    {
      for (int i = 0; i < cnt_plug; i++)
      {
        rc[i].switchOFF();
        delay(5);
      }
      
      turnOffTries++;
    }

    led.setFunction(Fade);
    led.run();
    
    break;
  case running:
    handler.setAdvanceTime(2400);
    if(turnOffTries)turnOffTries = 0;

    if(nextPlug>=0){
      // Serial.print("--------Begin Circle-------- \nCurrent Plug:");
      // Serial.println(nextPlug);
      rc[nextPlug].switchON();
      delay(5);
      int prev_p = handler.previousPlug(1);
      rc[prev_p].switchOFF();
      // Serial.print("Previous PLug: ");
      // Serial.println(prev_p);
      // Serial.println("--------End of Circle--------");


    }
    break;
  
  default:
  state = standby;
    break;
  }
}


int turnOffTries = 0;

void loop() {
  but.setLogic();
  but.printLogic();
  if(but.getLogic())statemamschine(running);
  else statemamschine(standby);
  
}

