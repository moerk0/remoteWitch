#ifndef LIGHT_H
#define LIGHT_H

#include <Arduino.h>

enum LightFunctions{
    solid,
    fade,
};


class Light
{
private:
    uint8_t _pin;
    uint64_t _last_change;
    uint8_t _state;
    uint8_t _fade_val;
    uint8_t _function;

    void fade(int steps, uint16_t interval);
    void writeOutput();
public:
    Light(uint8_t pin );
    void run(uint16_t interval);

};


#endif