#ifndef GLOBALS_H 
#define GLOBALS_H

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


#endif
