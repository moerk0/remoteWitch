
#include <Arduino.h>
#include <RGB_LED.h>
#include <pitchToNote.h>
#include <NewPing.h>
  #include <TM1637Display.h>

#include "globals.h"
#include "config.h"
#include "RCSwitch.h"
#include "Button.h"
#include "chase.h"
#include "light.h"
#include "filter.h"
#include "helper.h"


  TM1637Display display(CLK, DIO);
Button but(BUTTON_PIN,but0,LONG_PRESS_T);
RGB_LED rgb(RED_PIN, GREEN_PIN,BLUE_PIN);
ChasePLUGS handler(cnt_plug,DEFAULT_DELAY);
NewPing sonar(TRIG_PIN,ECHO_PIN,MAX_DIST);uint16_t result_cm;
Light licht(BUTTON_LED_PIN);
RCSwitch rc[] = {
  RCSwitch(RC_PIN,ADDR_1,TASTE_A),RCSwitch(RC_PIN,ADDR_1,TASTE_B),
  RCSwitch(RC_PIN,ADDR_2,TASTE_A),RCSwitch(RC_PIN,ADDR_2,TASTE_B),
  RCSwitch(RC_PIN,ADDR_3,TASTE_A),RCSwitch(RC_PIN,ADDR_3,TASTE_B),
  };

/*
CC_X = Freq
CC_X+1 = gain
CC X+2 = bandwith
CC_Y =Freq
*/
Filter lpf(CC_FIRST);
Filter bpf[N_BPF]= {Filter(CC_FIRST+3),Filter(CC_FIRST+6),Filter(CC_FIRST+9)};


void setup() {
  Serial.begin(115200);
  rgb.setFunction(Fade);

    display.setBrightness(0x0f);
    display.clear();
  
  sonar.pingTimer = millis(); // Start ping time now.
  randomSeed(analogRead(A0));

//setting of Filters. 
lpf.change(freqency,20);
  for(int i = 0; i<N_BPF;i++){
    if (i%2 == 0)bpf[i].sinus(gain); //set even numbers ascending =false
    // bpf[i].normalize(gain);         // all values easy
    
  }
}


void sonarTask() { // Timer2 interrupt calls this function every 24uS where you can check the ping status.
  if (sonar.check_timer()) { // This is how you check to see if the ping was received.
    // Here's where you can add code.
  result_cm = sonar.convert_cm(sonar.ping_result);
  sonarMsg(result_cm);
  lpf.change(freqency, mapSonarVal(result_cm));
    display.showNumberDec(result_cm);

  }


}
void checkSonar(){
   if (millis() >= sonar.pingTimer) {   // pingSpeed milliseconds since last ping, do another ping.
    sonar.pingTimer += PING_INTERVAL;      // Set the next ping time.
    sonar.ping_timer(sonarTask); // Send out the ping, calls "echoCheck" function every 24uS where you can check the ping status.
  }
}



void plugTask(uint8_t state){
  
  switch (state)
  {
    case default_off:
    {
      int nextPlug = handler.getNextPlug(chase);
      if (nextPlug>=0){
          handler.IdleIntervalHandler(500,5000,5*cnt_plug); // sends (5*cntplug) times with 500ms distance, then 5000k
          rc[nextPlug].switchOFF();
      }
    break;
    }

    case default_on:
    {
      int nextPlug = handler.getNextPlug(chase);
      if (nextPlug>=0){
        handler.IdleIntervalHandler(1000,10000,5*cnt_plug);
        rc[nextPlug].switchON();
        }
    break;
    }
  
    case chaotic:
    {
      int nextPlug = handler.getNextPlug(randomIndexChase);

      if(nextPlug>=0){
        handler.setAdvanceTime(random(100,2500));
        rc[nextPlug].switchON();
        delay(1);
        int prev_p = handler.previousPlug(1);
        rc[prev_p].switchOFF();
      }
    break;
    }
  }
}

bool doonce = false;

void statemaschine(byte fsm_state){
  switch (fsm_state)
  {
  case standby:
  {
    plugTask(default_off);
    display.setSegments(SEG_Stnd);
    rgb.setFunction(Fade);
    rgb.run();
    licht.fade();
    
    break;
  }
  case running:
  {
    licht.off();

    // plugTask(default_on);
    checkSonar();
    break;
  }
  case chaotic:
  {
    randomSeed(analogRead(A0));
    display.setSegments(SEG_CAOS); 
    // while (millis()-kaos < t)
    
      licht.blink(random(10,1000));
      // plugTask(chaotic);
      checkSonar();

      for (int i = 0; i < N_BPF; i++)
      {
        bpf[i].automate(gain,new_interval,10);
        // bpf[i].automate(freqency,random_interval,)
      }
      
    
    break;
    }
  }
}

void loop() {
  but.update();
  but.debugMsg();
  but.printLogic(MIDI_CH,pitchC3,255,&noteOn);
 MidiUSB.flush();

  // reset try counter after button is pushed
  if(!but.getVolantile())handler.IdleIntervalHandler(0,0,0);
  if(result_cm<CHAOS_THRESHOLD_CM)statemaschine(chaotic);
  
  if(but.getLong())display.showNumberDec(9999);
  else if(but.getLogic())statemaschine(running);
  else statemaschine(standby);
  
}

