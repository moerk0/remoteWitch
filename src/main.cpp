
#include <Arduino.h>
#include <RGB_LED.h>
#include <MIDIUSB.h>
#include <pitchToNote.h>
#include <NewPing.h>

#include "globals.h"
#include "config.h"
#include "RCSwitch.h"
#include "Button.h"
#include "chase.h"
#include "light.h"
#include "midi_helper.h"




Button but(BUTTON_PIN,but0,BUTTON_DEBOUNCE);
RGB_LED rgb(RED_PIN, GREEN_PIN,BLUE_PIN);
ChasePLUGS handler(cnt_plug,DEFAULT_DELAY);
NewPing sonar(TRIG_PIN,ECHO_PIN,MAX_DIST);
Light licht(BUTTON_LED_PIN);
RCSwitch rc[] = {
  RCSwitch(RC_PIN,ADDR_1,TASTE_A),RCSwitch(RC_PIN,ADDR_1,TASTE_B),
  RCSwitch(RC_PIN,ADDR_2,TASTE_A),RCSwitch(RC_PIN,ADDR_2,TASTE_B),
  RCSwitch(RC_PIN,ADDR_3,TASTE_A),RCSwitch(RC_PIN,ADDR_3,TASTE_B),
  };


void printSonar() { // Timer2 interrupt calls this function every 24uS where you can check the ping status.
  if (sonar.check_timer()) { // This is how you check to see if the ping was received.
    // Here's where you can add code.
   Serial.println(sonar.ping_result / US_ROUNDTRIP_CM);
  }


}
void sonarTask(){
   if (millis() >= sonar.pingTimer) {   // pingSpeed milliseconds since last ping, do another ping.
    sonar.pingTimer += PING_INTERVAL;      // Set the next ping time.
    sonar.ping_timer(printSonar); // Send out the ping, calls "echoCheck" function every 24uS where you can check the ping status.
  }
}









void setup() {
  Serial.begin(115200);
  rgb.setFunction(Fade);
  
  sonar.pingTimer = millis(); // Start ping time now.
  randomSeed(analogRead(A0));
}





void plugTask(uint8_t state){
  
  switch (state)
  {
    case default_off:
    {
      int nextPlug = handler.getNextPlug(chase);
      
      
      if (nextPlug>=0){
        
        
          handler.IdleIntervalHandler(500,5000,5*cnt_plug); // sends (5*cntplug) times with 500ms distance, then 5000k
          Serial.println(handler.advanceTime());
          
          rc[nextPlug].switchOFF();
      }
    break;
    }
    case default_on:
    {
      int nextPlug = handler.getNextPlug(chase);
      if (nextPlug>=0){

        handler.IdleIntervalHandler(1000,10000,5*cnt_plug);
        Serial.println(handler.advanceTime());
        rc[nextPlug].switchON();
        }

      }
    break;
  
    case chaotic:
    {
      int nextPlug = handler.getNextPlug(randomIndexChase);

      if(nextPlug>=0){
        handler.setAdvanceTime(random(100,2500));
        Serial.println(handler.advanceTime());
        rc[nextPlug].switchON();
        delay(5);
        int prev_p = handler.previousPlug(1);
        rc[prev_p].switchOFF();
      }
    break;
    }
  }
  
}


void statemaschine(byte fsm_state){
  switch (fsm_state)
  {
  case standby:
  {
    plugTask(default_off);
    rgb.setFunction(Fade);
    rgb.run();
    licht.run();
    
    break;
  }
  case running:
  {
    licht.off();
    rgb.setFunction(Fade);
    plugTask(default_on);
    sonarTask();
    break;
  }
  default:

    break;
  }
}


int turnOffTries = 0;

void loop() {
  but.setLogic();
  but.printLogic(MIDI_CH,pitchC3,255,&noteOn);
 

  // reset try counter after button is pushed
  if(!but.getVolantile())handler.IdleIntervalHandler(0,0,0);
  
  if(but.getLogic())statemaschine(running);
  else statemaschine(standby);
  MidiUSB.flush();
}

