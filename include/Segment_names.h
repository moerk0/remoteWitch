#include <TM1637Display.h>
//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D

const uint8_t SEG_SETU[] = {
	SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,          // S
  SEG_A | SEG_F | SEG_G | SEG_E | SEG_D,           // E
  SEG_F | SEG_G | SEG_E,                          // t
  SEG_F | SEG_E | SEG_D | SEG_C | SEG_B,           // P

};

const uint8_t SEG_CAOS[] = {
	SEG_A | SEG_F | SEG_E | SEG_D ,                    // C
	SEG_E | SEG_F | SEG_G | SEG_A | SEG_B | SEG_C ,   // A
	SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
	SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,          // S
	};

const uint8_t SEG_Stnd[] = {
	SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,          // S
  SEG_F | SEG_G | SEG_E,                          // t
  SEG_E | SEG_G | SEG_C,                          // n
  SEG_E | SEG_G | SEG_C | SEG_D | SEG_B           // d
};
 
const uint8_t SEG_run[] = {
  SEG_E | SEG_G,
  SEG_E | SEG_D | SEG_C,
  SEG_E | SEG_G | SEG_C,

};