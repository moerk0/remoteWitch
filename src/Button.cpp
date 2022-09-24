#include <Arduino.h>
#include <Button.h>

Button::Button(uint8_t but_pin,uint8_t but_address, uint16_t longTime, uint8_t max_increment)
    :   pin(but_pin)
    ,   address(but_address)
    ,   debounceT(BUTTON_DEBOUNCE)
    ,   longPressTime(longTime)
    ,   gate_logic(true)
    ,   gate_print(true)
    ,   max_cnt(max_increment)
    {
        pinMode(pin, INPUT_PULLUP);
    }

void Button::debugMsg(){
    Serial.print("Volantile: ");
  Serial.print(getVolantile());
  Serial.print(" | Logic: ");
  Serial.print(getLogic());
  Serial.print(" | Long: ");
  Serial.print(getLong());
  Serial.print(" | Increment: ");
  Serial.print(getLong());
  Serial.println();
}

void Button::update(){
    if (millis() - this->lastDebounce > this->debounceT){ //Debounce Timer
        this->state = digitalRead(this->pin);
        
        if (!this->state && this->state_prev){ //Button pushed
            this->beginPressTimer = millis(); //start long press Timer
            increment();
            
        }
        else if (!this->state && !this->state_prev){ //Check if the button is still pressed
            if( millis()-beginPressTimer > longPressTime){ // Check if it has been pushed for long enough
                if(this->gate_long)this->state_long = !this->state_long; //Change state
                this->gate_long = false;                                // prevent oscillation
            }
        }
        else if(this->state && !state_prev){ //button released
            if(this->gate_long)setLogic(); //check if long button has been toggled, if so don't set logic
            this->gate_long = true;         //open gate to toggle long press
            // this->gate =true;
        }
        


        this->state_prev = this->state;
        this->lastDebounce = millis();     
    }
    
}

void Button::increment(){
    (cnt < max_cnt)?cnt++:cnt=0;
    }
    

void Button::setLogic(){
(this->gate_logic)?this->state_logic = HIGH:this->state_logic = LOW; //set logic state
            this->gate_logic = !this->gate_logic;
}

//Deprected
void Button::setVolantile(){
    if (millis() - this->lastDebounce > this->debounceT){
        this->state = digitalRead(this->pin);
        this->state_prev = this->state;
        this->lastDebounce = millis();     
    }

    if (this->state_logic){ // deactivate effect in case it was active when layer switched
        this->state_logic = !this->state_logic;
        this->gate_logic = !this->gate_logic;
    }
}


void Button::printLogic(byte a, byte b, byte c,void(*func)(byte,byte,byte)){
    if (this->state_logic && this->gate_print) {
        Serial.println("e" + String(this->address) + ":" + String(this->state_logic)); 
        this->gate_print = !this->gate_print;
        func(a,b,c);
    }

    if (!this->state_logic && !this->gate_print){
        Serial.println("e" + String(this->address) + ":" + String(this->state_logic));   
        this->gate_print = !this->gate_print;
        func(a,b,c);
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