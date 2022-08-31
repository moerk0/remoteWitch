#include "light.h"
#include <Arduino.h>

Light::Light(uint8_t pin):_pin(pin), _last_change(0)
{

    pinMode(_pin, OUTPUT);
}

void Light::fade(int steps, uint16_t interval){
        static uint8_t reverse;
        if(_fade_val < 255 && !reverse)_fade_val+=steps;
        else reverse = true;
        
        if(reverse){
            _fade_val -= steps;
            if(_fade_val<=steps)reverse = false;
        }

}

void Light::run(uint16_t interval){
    if (millis() - this->_last_change >= interval){
        fade(10,interval);
        writeOutput();
        _last_change += interval;
    }
}

void Light::writeOutput(){
    analogWrite(_pin,_fade_val);
}

