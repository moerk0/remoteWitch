#ifndef BUTTON_H
#define BUTTON_H
#include <Arduino.h>

class Button
{
    private:

        int pin;
        int address;

        bool state;
        bool state_prev;
        bool state_logic;
        bool gate;
        bool gate_print;

        int debounceT;
        unsigned long lastDebounce;


    public:
    Button(int pin, int but_address, int debounceTime);

    void setLogic();
    void setVolantile();
    void setLastDebounce();

    bool getLogic(){return state_logic;}
    bool getVolantile(){return state;}

    void printLogic();
    void printVolantile();

};





#endif