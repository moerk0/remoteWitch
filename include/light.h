
#ifndef LIGHT_H
#define LIGHT_H

#include <Arduino.h>

class Light
{
private:
    const uint8_t _pin;
    uint8_t _fade_val;
    uint64_t _last_change;
    uint8_t _fade_steps;
    uint16_t _fade_interval;
    uint16_t _interval;
    bool _descending;

    void calcNextStep();
    void writeOutput();
    
    void startTimer(uint16_t);
    bool checkTimer();
public:
    Light(uint8_t pin );
    void setFadeOptions(uint8_t steps, uint16_t interval);
    void setInterval(uint16_t t){_interval = t;}
    void off();
    void toggle();
    
    void blink();
    void blink(uint16_t interval);
    void blink(uint16_t onT, uint16_t offT);
    void fade();

};
#endif