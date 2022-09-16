#ifndef CONFIG_H 
#define CONFIG_H

//Pin Config
#define RC_PIN 13
#define BUTTON_PIN 7
#define RED_PIN 6
#define GREEN_PIN 9
#define BLUE_PIN 5
#define BUTTON_LED_PIN 10
#define TRIG_PIN 11
#define ECHO_PIN 12
#define CLK 2
#define DIO 3

//Sonar config
#define MAX_DIST 70
#define PING_INTERVAL 50
#define CHAOS_THRESHOLD_CM 10

// MIDI Config
#define MIDI_CH 0
#define N_BPF 3
#define CC_FIRST 10

// PLug config
#define DEFAULT_DELAY 2400

//Button Config
#define LONG_PRESS_T 500


#endif
