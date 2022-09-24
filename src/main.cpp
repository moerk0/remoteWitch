
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

struct FSM fsm;
  TM1637Display display(CLK, DIO);
Button but(BUTTON_PIN,but0,LONG_PRESS_T);
// RGB_LED rgb(RED_PIN, GREEN_PIN,BLUE_PIN);
ChasePLUGS handler(N_PLUGS,DEFAULT_DELAY);
NewPing sonar(TRIG_PIN,ECHO_PIN,MAX_DIST);uint16_t result_cm;
Light licht(BUTTON_LED_PIN);
RCSwitch rc[N_PLUGS] = {
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
Filter lpf( FIRST_CC);
Filter bpf[N_BPF]= {Filter( FIRST_CC+3),Filter( FIRST_CC+6),Filter( FIRST_CC+9)};
void plugCountError(){
  while (1)
      {
        display.showNumberDec(0,true);
        delay(100);
        display.clear();
        delay(100);
      }
}

void setup() {
  Serial.begin(115200);
  // rgb.setFunction(Fade);

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
  fsm.state = standby;
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



bool plugTask(uint8_t state){
  switch (state)
  {
    case default_off:
    {
      int nextPlug = handler.getNextPlug(chase);
      if (nextPlug>=0){
          rc[nextPlug].switchOFF();
          handler.IdleIntervalHandler(1000,5000,10);
      }
      if (nextPlug>=N_PLUGS-1) return true;
      else return false;
    break;
    }

    case default_on:
    {
      int nextPlug = handler.getNextPlug(chase);
      if (nextPlug>=0){
        rc[nextPlug].switchON();
        handler.IdleIntervalHandler(500,5000, 5*N_PLUGS);
        }
      if (nextPlug>=N_PLUGS-1) return true;
      else return false;
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
      }
      return false;
    break;
    }
  }
}

bool doonce = false;

void statemaschine(FSM *p){
  switch (p->state)
  {
  case standby:
  {
    plugTask(default_off);
    // rgb.run();
    licht.fade();
    p->completed[running] = false;
    p->completed[chaos] = false;
    
    break;
  }
  case running:
  {
    plugTask(default_on);
    checkSonar();
     p->completed[standby] = false;
    p->completed[chaos] = false;
   
    break;
  }
  case chaotic:
  {
    display.setSegments(SEG_CAOS); 
    
      licht.blink(1);
      plugTask(chaotic);
      checkSonar();

      for (int i = 0; i < N_BPF; i++)
      {
        bpf[i].automate(gain,new_interval,10);
        // bpf[i].automate(freqency,random_interval,)
      }
      
    
     p->completed[standby] = false;
    p->completed[running] = false;
    break;
    }
    case cnt_states:
    break;
  }
  // for(byte i = 0; i<cnt_states;i++){
  //   if (i != p->state)
  //   {
  //     p->completed[i] = false;
  //   }
    
  // }
}

void trans2stnd(struct FSM *p){
  if(!p->completed[standby]){
    p->state = cnt_states;
    display.setSegments(SEG_Stnd);
    // rgb.setFunction(Fade);
    handler.resetIntervalHandler();
    handler.IdleIntervalHandler(500,5000, 5*N_PLUGS);
    noteOn(MIDI_CH,pitchC3,255);
    MidiUSB.flush();

    p->completed[standby] = true;
  }
  else p->state = standby;
}

void trans2run(struct FSM *p){
  if(p->completed[running] == false){
    p->state = cnt_states;
    display.setSegments(SEG_run); 
    noteOn(MIDI_CH,pitchC3,255);
    noteOn(MIDI_CH,pitchD3,255);
    MidiUSB.flush();
    handler.resetIntervalHandler();
    licht.off();

    p->completed[running] = true;
  }
  else p->state = running;
    
}

void trans2chaos(struct FSM *p){
  if(!p->completed[chaos]){
    p->state = cnt_states;
    display.setSegments(SEG_CAOS); 
    randomSeed(analogRead(A0));
    noteOn(MIDI_CH,pitchC3,255);
    noteOn(MIDI_CH,pitchD3,255);
    MidiUSB.flush();
    p->completed[chaos] = true;
  }
  else p->state = chaos;
}

void loop() {
  but.update();
  
  if(but.getLogic())(result_cm<CHAOS_THRESHOLD_CM)?trans2chaos(&fsm):trans2run(&fsm);
  else trans2stnd(&fsm);
  statemaschine(&fsm);
  
}

