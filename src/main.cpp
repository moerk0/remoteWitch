
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
#include "Segment_names.h"

struct FSM fsm;
TM1637Display display(CLK, DIO);
Button but(BUTTON_PIN,but0,LONG_PRESS_T);
// RGB_LED rgb(RED_PIN, GREEN_PIN,BLUE_PIN);
ChasePLUGS handler(N_PLUGS,DEFAULT_DELAY);
NewPing sonar(TRIG_PIN,ECHO_PIN,MAX_DIST);uint16_t result_cm;
Light licht(BUTTON_LED_PIN);
RCSwitch rc[N_PLUGS] = {
  RCSwitch(RC_PIN,ADDR_1,TASTE_A),
  RCSwitch(RC_PIN,ADDR_1,TASTE_B),
  RCSwitch(RC_PIN,ADDR_2,TASTE_A),
  RCSwitch(RC_PIN,ADDR_2,TASTE_B),
  RCSwitch(RC_PIN,ADDR_3,TASTE_A),
  RCSwitch(RC_PIN,ADDR_3,TASTE_B),
  };

/*
CC_X = Freq
CC_X+1 = gain
CC X+2 = bandwith
CC_Y =Freq
*/
Filter lpf( FIRST_CC);
Filter bpf[N_BPF]= {Filter( FIRST_CC+3),Filter( FIRST_CC+6),Filter( FIRST_CC+9)};

void plugerror(){
  size_t len = sizeof(rc) / sizeof(rc[0]);
  if (len != N_PLUGS)
  {
    while (1)
    {
      display.setSegments(SEG_ERR);
      delay(100);
      display.clear();
      delay(100);
      Serial.print("len: ");
      Serial.print(len);
      Serial.print(" does not match N: ");
      Serial.println(N_PLUGS);
    }
  }
}

void setup() {
  Serial.begin(115200);
  // rgb.setFunction(Fade);

  display.setBrightness(0x0f);
  display.clear();
  plugerror();
  
  sonar.pingTimer = millis(); // Start ping time now.

//setting of Filters. 
  lpf.change(freqency,20);
  for(int i = 0; i<N_BPF;i++){
    if (i%2 == 0)bpf[i].sinus(gain); //set even numbers ascending =false
    // bpf[i].normalize(gain);         // all values easy
  }

  fsm.state = standby;
  display.setSegments(SEG_Stnd);
}


void sonarTask() { // Timer2 interrupt calls this function every 24uS where you can check the ping status.
  if (sonar.check_timer()) { // This is how you check to see if the ping was received.
    // Here's where you can add code.
    result_cm = sonar.convert_cm(sonar.ping_result);
    // sonarMsg(result_cm);
    lpf.change(freqency, mapSonarVal(result_cm));
    display.showNumberDec(result_cm);

  }


}
void checkSonar(NewPing *p){
   if (millis() >= p->pingTimer) {   // pingSpeed milliseconds since last ping, do another ping.
    p->pingTimer += PING_INTERVAL;      // Set the next ping time.
    p->ping_timer(sonarTask); // Send out the ping, calls "echoCheck" function every 24uS where you can check the ping status.
  }
}



void plugTask(uint8_t state){
  switch (state)
  {
    case default_off:
    {
      int nextPlug = handler.getNextPlug(chase);
      if (nextPlug>=0){
          rc[nextPlug].switchOFF();
          handler.IdleIntervalHandler(1000,5000,5*N_PLUGS);
          #if PLUG_MSG == true
            rc[nextPlug].send2Monitor();
          #endif
      }
      
    break;
    }

    case default_on:
    {
      int nextPlug = handler.getNextPlug(chase);
      if (nextPlug>=0){
        rc[nextPlug].switchON();
        handler.IdleIntervalHandler(500,5000, 5*N_PLUGS);
        #if PLUG_MSG == true
            rc[nextPlug].send2Monitor();
        #endif
        }
    break;
    }
  
    case chaotic:
    {
      int nextPlug = handler.getNextPlug(randomIndexChase);

      if(nextPlug>=0){
        handler.setAdvanceTime(random(500,2500));
        rc[nextPlug].switchON();
        delay(1);
        int prev_p = handler.previousPlug(1);
        rc[prev_p].switchOFF();

        #if PLUG_MSG == true
          rc[nextPlug].send2Monitor();
          rc[prev_p].send2Monitor();
        #endif
      }
    break;
    }
  }
}


void trans2run(){
  display.setSegments(SEG_run); 
  noteOn(MIDI_CH,pitchC3,255);
  noteOn(MIDI_CH,pitchD3,255);
  MidiUSB.flush();
  handler.resetIntervalHandler();
  licht.off();
}

void trans2chaos(){
  display.setSegments(SEG_CAOS); 
  randomSeed(analogRead(A0));
  noteOn(MIDI_CH,pitchC3,255);
  noteOn(MIDI_CH,pitchD3,255);
  MidiUSB.flush();
}

void trans2stnd(){
  display.setSegments(SEG_Stnd);
    // rgb.setFunction(Fade);
  handler.resetIntervalHandler();
  noteOn(MIDI_CH,pitchC3,255);
    
}

void statemaschine(FSM *p){
  switch (p->state)
  {
  case standby:
  
    plugTask(default_off);
    // rgb.run();
    licht.fade();
    
    break;
  
  case running:
  
    plugTask(default_on);
    checkSonar(&sonar);   
    break;
  
  case chaos:
  
    display.setSegments(SEG_CAOS); 
    
      licht.blink(10);
      plugTask(chaotic);
      checkSonar(&sonar);

      for (int i = 0; i < N_BPF; i++)
      {
        bpf[i].automate(gain,new_interval,10);
        // bpf[i].automate(freqency,random_interval,)
      }
    break;

  }
}

void transit2(byte to_state,FSM *p){
  if(to_state == running)resetKaosTimer(&fsm);

  if(p->state != to_state){
    if(p->state == standby && to_state == running) trans2run();
    else if((p->state ==running || p->state==chaos) && to_state == standby) trans2stnd();
    
    else if(p->state == running && to_state == chaos){
      beginKaosTimer(&fsm);
      
      if(checkKaosTimer(&fsm)) trans2chaos();
      else return;
    }

    p->state_prev = p->state;
    p->state = to_state;
  }
}

void loop() {
  but.update();
  
  if(but.getLogic())(result_cm<CHAOS_THRESHOLD_CM)?transit2(chaos,&fsm):transit2(running,&fsm);
  else transit2(standby,&fsm);
  statemaschine(&fsm);
}

