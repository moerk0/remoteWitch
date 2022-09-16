#include <Arduino.h>
#include "light.h"



Light::Light(uint8_t pin):_pin(pin), _fade_val(0), _last_change(millis())
{
    setFadeOptions(10,50);
    pinMode(_pin, OUTPUT);
}


void Light::startTimer(uint16_t t){
    _last_change = millis();
    _interval = t;
}
bool Light::checkTimer(){
    if (millis() - _last_change >= _interval)return true;
    else return false;
}

void Light::blink(uint16_t interval){
    if(checkTimer()){
        startTimer(interval);
        toggle();
    }
}

void Light::blink(uint16_t onT, uint16_t offT){
    if(checkTimer()){
        if(!digitalRead(_pin))startTimer(onT);
        else startTimer(offT);
        toggle();
    }
}

void Light::blink(){
    if(checkTimer()){
        startTimer(_interval);
        toggle();
    }
}

void Light::fade(){
    if (checkTimer())
    {
        startTimer(_fade_interval);
        calcNextStep();
        analogWrite(_pin, _fade_val);
    }
    
}

void Light::setFadeOptions(uint8_t steps, uint16_t interval){
    _fade_steps = steps;
    _fade_interval = interval;
}

void Light::calcNextStep(){
    if(!_descending){
        if(_fade_val + _fade_steps <= 255){
            _fade_val += _fade_steps;
        }
    
        else{
            _fade_val = 255;
            _descending = true;
        }
    }
    else{
        if(_fade_val - _fade_steps >=0){
            _fade_val -= _fade_steps;
        }
        else{
            _fade_val = 10;
            _descending = false;
        }
    }

}

void Light::off(){
    digitalWrite(_pin, 0);
}

void Light::toggle(){
    digitalWrite(_pin, !digitalRead(_pin));
}

