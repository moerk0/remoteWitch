#ifndef GLOBALS_H 
#define GLOBALS_H

enum MidiChannels{
  hpf,
  bpf_1,
  bpf_2,
  bpf_3,
  play_pause,
  toggle_mute,
  cc_cnt,
};

enum FSMStates{
  praeludium,
  standby,
  running,
  chaos,
  midi_setup,
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
