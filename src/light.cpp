#include <Arduino.h>
#include "light.h"



Light::Light(uint8_t pin):_pin(pin), _fade_val(0), _last_change(millis()), _function(linearFade)
{
    setFadeOptions(10,50);
    pinMode(_pin, OUTPUT);
}

void Light::run(){
    if(millis() - _last_change > _fade_interval){
        switch (_function)
        {
        case linearFade:
            fade();
            writeOutput();
            break;
        case blink:
            toggle();
        case logarithmicFade:
            digitalWrite(_pin, logSleep());
        }
        _last_change = millis();
    }
}

void Light::writeOutput(){
    analogWrite(_pin, _fade_val);
}

void Light::setFadeOptions(uint8_t steps, uint16_t interval){
    _fade_steps = steps;
    _fade_interval = interval;
}

void Light::fade(){
    static bool descending;
    if(!descending){
        if(_fade_val + _fade_steps <= 255){
            _fade_val += _fade_steps;
        }
    
        else{
            _fade_val = 255;
            descending = true;
        }
    }
    else{
        if(_fade_val - _fade_steps >=0){
            _fade_val -= _fade_steps;
        }
        else{
            _fade_val = 10;
            descending = false;
        }
    }

}

void Light::off(){
    digitalWrite(_pin, 0);
}

void Light::toggle(){
    digitalWrite(_pin, !digitalRead(_pin));
}

uint16_t Light::logSleep(){
    _pwm_interval = 100;
    static float r = (_pwm_interval * log10(2))/(log10(255));
    static uint8_t interval;
    if(interval >= _pwm_interval)interval = 0;
    return pow(2, (interval++ / r)) - 1;

}