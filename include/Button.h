#ifndef BUTTON_H
#define BUTTON_H
#include <Arduino.h>

#define BUTTON_DEBOUNCE 30

class Button
{
    private:

        const uint8_t pin;
        const uint8_t address;    // Assign a meaningful number to each Button object, so you can print them.

        const uint16_t debounceT;
        uint64_t lastDebounce;

        bool state;
        bool state_prev;
        bool state_logic;
        bool gate;
        bool gate_print;



    public:
    Button(uint8_t pin, uint8_t but_address, uint16_t debounceTime);

    void setLogic();
    void setVolantile();
    void setLastDebounce();

    bool getLogic(){return state_logic;}
    bool getVolantile(){return state;}

    void printLogic(byte a, byte b ,byte c,void(*func)(byte,byte,byte));
    void printVolantile();

};





#endif