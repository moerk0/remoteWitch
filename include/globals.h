#ifndef GLOBALS_H 
#define GLOBALS_H
#include "config.h"

enum FSMStates{
  standby,
  running,
  chaos,
};

enum PlugStates{
  idle,
  default_on,
  default_off,
  chaotic,
};

enum SonarStates{
    nothing_in_range, //check if 0, 
    measure, //check if measurement is in bound
    exclude, // if unwanted numbers appear
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

// const uint8_t SEG_CAOS[] = {
// 	SEG_A | SEG_F | SEG_E | SEG_D ,                    // C
// 	SEG_E | SEG_F | SEG_G | SEG_A | SEG_B | SEG_C ,   // A
// 	SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
// 	SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,          // S
// 	};



#endif
