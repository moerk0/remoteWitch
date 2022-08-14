#include <Arduino.h>
#include "RCSwitch.h"


RCSwitch::RCSwitch(uint8_t pin)
    :   txPin(pin)
    ,   pulse_lenght_short(PULSE_LENGHT)
    ,   pulse_lenght_long (PULSE_LENGHT * 3)
    ,   repitions(10)
{
    pinMode(txPin, OUTPUT);
}

void RCSwitch::sendLOW(){
  digitalWrite(this->txPin, HIGH);
  delayMicroseconds(this->pulse_lenght_short);
  digitalWrite(this->txPin, LOW);
  delayMicroseconds(this->pulse_lenght_long);
}
void RCSwitch::sendHIGH(){
  digitalWrite(this->txPin, HIGH);
  delayMicroseconds(this->pulse_lenght_long);
  digitalWrite(this->txPin, LOW);
  delayMicroseconds(this->pulse_lenght_short);
}


//see header File for further information
void RCSwitch::collectPhrase(uint8_t adrr, uint8_t taste, uint8_t state){
  
  //Reset phrase and Pos
  this->nPos = 1;  //  nPos = 0, always startBit 0
  for (uint8_t i = 0; i < PHRASE_LENGHT; i++)
  {
    this->phrase[i] = 0;
  }
  
  //write address bits,
  for (uint8_t i = 0; i < 5; i++) {
    if ( adrr & ( 1 << i ) ) {
      this->phrase[this->nPos++] = 1;
      this->nPos++;
    }
    else {
      this->nPos += 2;
      
    }
  }

  // write taste bits
  for (uint8_t i = 0; i < 5; i++) {
        if (taste & (1 << i)) {
            this->nPos += 2;
        }
        else {
            this->phrase[this->nPos++] = 1;
            this->nPos++;
        }
    }

  //write state
    if (state){

        this->nPos += 2;
        this->phrase[this->nPos] = 1;
    }

    else{
        this->phrase[this->nPos] = 1;
    }
}


void RCSwitch::transmitPhrase(){
    for ( int i = 0; i < this->repitions; i++ ){

        for ( int i = 0; i < PHRASE_LENGHT; i++ ){

            if ( this->phrase[i] )
                sendHIGH();
            

            else
                sendLOW();
        }
        delay(10);
    }  
}

void RCSwitch::send2Monitor(){

    for (int i = 0; i < PHRASE_LENGHT; i++){

        Serial.print(this->phrase[i]);
        
        if ( i%2 == 0 )
            Serial.print(" ");
        
        if ( i%10== 0 )
            Serial.print('\t');
    
    }
    Serial.println();
}

void RCSwitch::switchON(uint8_t addr, uint8_t taste){
    collectPhrase( addr, taste, true );
    transmitPhrase();
    send2Monitor();
}
void RCSwitch::switchOFF(uint8_t addr, uint8_t taste){
    collectPhrase( addr, taste, false );
    transmitPhrase();
    send2Monitor();
}
