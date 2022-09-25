#ifndef GLOBALS_H 
#define GLOBALS_H

enum FSMStates{
  standby,
  running,
  chaos,
  // midi_setup,
  cnt_states,
};

enum PlugStates{
  default_off,
  default_on,
  chaotic,
  cnt,
};

enum Buttons{
  but0
};

 struct FSM
{
  uint8_t state;
  uint8_t state_prev;
  bool completed[cnt_states];
  bool timer_running;
  uint64_t kaosT;
};




#endif
