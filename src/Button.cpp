#include <Arduino.h>
#include <Button.h>

Button::Button(int but_pin,int but_address, int debounce_time)
    :   pin(but_pin)
    ,   address(but_address)
    ,   debounceT(debounce_time)
    ,   state(true)
    ,   gate(true)
    ,   gate_print(true)
    {
        pinMode(pin, INPUT_PULLUP);
    }

void Button::setLogic(){
 if (millis() - this->lastDebounce > this->debounceT){ //Debounce Timer
  this->state = digitalRead(this->pin);

    if (this->state == LOW && this->state_prev == HIGH){ //Button pushed
        if (this->gate) //Magic conversion gate and logic state. 01 - 10
      {
        this->state_logic = HIGH;
      }

      else{
        this->state_logic = LOW;
      }

      this->gate = !this->gate;
    }

    this->state_prev = this->state;
    this->lastDebounce = millis();     
 }
}

void Button::setVolantile(){
    if (millis() - this->lastDebounce > this->debounceT){
        this->state = digitalRead(this->pin);
        this->state_prev = this->state;
        this->lastDebounce = millis();     
    }

    if (this->state_logic){ // deactivate effect in case it was active when layer switched
        this->state_logic = !this->state_logic;
        this->gate = !this->gate;
    }
}


void Button::printLogic(){
    if (this->state_logic && this->gate_print) {
        Serial.println("e" + String(this->address) + ":" + String(this->state_logic)); 
        this->gate_print = !this->gate_print;
    }

    if (!this->state_logic && !this->gate_print){
        Serial.println("e" + String(this->address) + ":" + String(this->state_logic));   
        this->gate_print = !this->gate_print;
    }
}

void Button::printVolantile(){
    if(this->state == LOW ){ 
        if (this->gate_print){ //Print HIGH
            Serial.println("s" + String(this->address) + ":" + String(!this->state));   
            this->gate_print = !this->gate_print;
        }
        
    }
    else{
        if (!this->gate_print){  // Print LOW
            Serial.println("s" + String(this->address) + ":" + String(!this->state));   
            this->gate_print = !this->gate_print;
        }
    }
}