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





#endif
