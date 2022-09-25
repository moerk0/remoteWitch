#ifndef CONFIG_H 
#define CONFIG_H

//Pin Config
#define RC_PIN 13
#define BUTTON_PIN 2
// #define RED_PIN 6
// #define GREEN_PIN 9
// #define BLUE_PIN 5
#define BUTTON_LED_PIN 10
#define TRIG_PIN 12
#define ECHO_PIN 11
#define CLK 4
#define DIO 3

//Sonar config
#define MAX_DIST 500
#define PING_INTERVAL 100

//Caos Config
#define CHAOS_THRESHOLD_CM 50
#define CHAOS_THRESHOLD_MS 1000

// MIDI Config
#define MIDI_CH 0
#define N_BPF 3
#define N_CC 4
#define N_NOTES 4
#define FIRST_CC 10
#define PLAY_PAUSE 48
#define TOGGLE_MUTE 49

// PLug config
#define DEFAULT_DELAY 2400
#define N_PLUGS 4

//Button Config
#define LONG_PRESS_T 500

//Debug Messages
#define PLUG_MSG        1
#define MIDI_MSG        0
#define CHAOS_TIMER_MSG false
#define BUTTON_MSG      0

#endif
