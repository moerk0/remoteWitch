
#ifndef LIGHT_H
#define LIGHT_H

#include <Arduino.h>

enum LightFunctions{
    solid,
    blink,
    linearFade,
    logarithmicFade,

};


class Light
{
private:
    const uint8_t _pin;
    uint8_t _fade_val;
    uint64_t _last_change;
    uint8_t _fade_steps;
    uint16_t _fade_interval;
    uint8_t _function;
    uint8_t _pwm_interval;

    void fade();
    void writeOutput();
    uint16_t logSleep();
public:
    Light(uint8_t pin );
    void run();
    void setFadeOptions(uint8_t steps, uint16_t interval);
    void off();
    void toggle();

};
#endif