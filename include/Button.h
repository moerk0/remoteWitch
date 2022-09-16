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
        uint64_t beginPressTimer;
        uint16_t longPressTime;

        bool state;
        bool state_prev;
        bool state_logic;
        bool state_long;
        bool state_long_prev;
        bool gate;
        bool gate_long;
        bool gate_print;

        uint8_t cnt;
        uint8_t max_cnt;




    public:
    Button(uint8_t pin, uint8_t but_address, uint16_t longTime);

    void setLogic();
    void setVolantile();
    void setLastDebounce();
    void update();

    bool getLogic(){return state_logic;}
    bool getVolantile(){return state;}
    bool getLong(){return state_long;}
    void debugMsg();

    void printLogic(byte a, byte b ,byte c,void(*func)(byte,byte,byte));
    void printVolantile();

};





#endif