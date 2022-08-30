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

enum RunStates{
  standby,
  running,
};
uint8_t fsm_state;

enum PlugStates{
  idle,
  chaotic,
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
RGB_LED led(RED_PIN, GREEN_PIN,BLUE_PIN);
ChasePLUGS handler(cnt_plug,DEFAULT_DELAY);

RCSwitch rc[] = {
  RCSwitch(RC_PIN,ADDR_1,TASTE_A),RCSwitch(RC_PIN,ADDR_1,TASTE_B),
  RCSwitch(RC_PIN,ADDR_2,TASTE_A),RCSwitch(RC_PIN,ADDR_2,TASTE_B),
  RCSwitch(RC_PIN,ADDR_3,TASTE_A),RCSwitch(RC_PIN,ADDR_3,TASTE_B),
  };

void setup() {
 Serial.begin(115200);
 led.setFunction(Fade);

 randomSeed(analogRead(A0));
}
 void turnOffAllPlugs(){
  for (int i = 0; i < cnt_plug; i++)
    {
      rc[i].switchOFF();
      delay(5);
    }
 }

void plugTask(uint8_t state){
  static int turnOffTries;
  
  switch (state)
  {
    case idle:
    {
      int nextPlug = handler.getNextPlug(chase);
      
      
      if (nextPlug>=0){
        
        if(fsm_state == standby){
          handler.IdleIntervalHandler(500,5000,5*cnt_plug);
          Serial.println(handler.advanceTime());
          
          rc[nextPlug].switchOFF();
          turnOffTries++;
        }
        if(fsm_state == running){
          handler.IdleIntervalHandler(1000,10000,5*cnt_plug);
          Serial.println(handler.advanceTime());
          rc[nextPlug].switchON();
        }

      }
    break;
    }
  
    case chaotic:
    {
      int nextPlug = handler.getNextPlug(randomIndexChase);
      if(turnOffTries)turnOffTries = 0;

      if(nextPlug>=0){
        handler.setAdvanceTime(random(100,2500));
        Serial.println(handler.advanceTime());
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
    }
  }
}


void statemaschine(){
  switch (fsm_state)
  {
  case standby:
  {
    plugTask(idle);
    if(led.getFunction() != Fade) led.setFunction(Fade);
    led.run();
    
    break;
  }
  case running:
  {
    plugTask(idle);
    break;
  }
  default:

    break;
  }
}


int turnOffTries = 0;

void loop() {
  but.setLogic();
  but.printLogic();

  // reset try counter after button is pushed
  if(!but.getVolantile())handler.IdleIntervalHandler(0,0,0);
  
  if(but.getLogic())fsm_state = running;
  else fsm_state = standby;
  statemaschine();

  
}

