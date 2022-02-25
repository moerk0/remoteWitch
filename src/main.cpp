#include <Arduino.h>
#define PROTOCOL_LENGTH 25
#define PULSE_LENGHT 320


struct Protocol
{
  int rxPin = 2;
  int arr[PROTOCOL_LENGTH] = {0};
  int nPos;
  int pulse_lenght_short = PULSE_LENGHT;
  int pulse_lenght_long = PULSE_LENGHT*3;
  int repitions = 10;
};

struct Protocol protocol;

void sendLOW(struct Protocol *p){
  digitalWrite(p->rxPin, HIGH);
  delayMicroseconds(p->pulse_lenght_short);
  digitalWrite(p->rxPin, LOW);
  delayMicroseconds(p->pulse_lenght_long);
}
void sendHIGH(struct Protocol *p){
  digitalWrite(p->rxPin, HIGH);
  delayMicroseconds(p->pulse_lenght_long);
  digitalWrite(p->rxPin, LOW);
  delayMicroseconds(p->pulse_lenght_short);
}

/**
 * Adress can be any value between 0 & 31,
 * it has to match the Adress dip switches at the Stecker itself. 
 * Multiple Stecker can have the same Adress
 * 
 * Taste has to be either 1,2,4,8
 * it refers to a specific Stecker of a Group
 * */
void collect(int adrr, int taste, int state ,struct Protocol *p){
  
  //Reset Array an Pos
  p->nPos = 1;  //  nPos = 0, always startBit 0
  for (int i = 0; i < PROTOCOL_LENGTH; i++)
  {
    // (i%2 == 0) ? p->arr[i] = 0 :  p->arr[i] = 1;
    p->arr[i] = 0;
  }
  
  //write address bits, Adress can be any value between 0 & 31, it has to match the dips at the switch. 
  //All Switches need to be
  for (int i = 0; i < 5; i++) {
    if ( adrr & ( 1 << i ) ) {
      p->arr[p->nPos++] = 1;
      p->nPos++;
    }
    else {
      p->nPos += 2;
      
    }
  }

  // write taste bits
  for (int i = 0; i < 5; i++) {
    if (taste & (1 << i)) {
      p->nPos += 2;
    }
    else {
      p->arr[p->nPos++] = 1;
      p->nPos++;
    }
  }

  //write state
  if (state)
  {
    p->nPos += 2;
    p->arr[p->nPos] = 1;
  }

  else{
    p->arr[p->nPos] = 1;
  }
}

void transmitData(struct Protocol *p){
  for ( int i = 0; i < p->repitions; i++ ){
    
    for ( int i = 0; i < PROTOCOL_LENGTH; i++ ){

      if ( p->arr[i] ){
        sendHIGH(&protocol);
        // Serial.print('0');
      }

      else{
        sendLOW(&protocol);
        // Serial.print('1');
      }
    }
    delay(10);
  }  
    // Serial.println();
}

void sendDebug(struct Protocol *p){
  for (int i = 0; i < PROTOCOL_LENGTH; i++){

    Serial.print(protocol.arr[i]);
    if ( i%2 == 0 ){
      Serial.print(" ");
    }
    if ( i%10== 0 ){
      Serial.print('\t');
    }
  }
  Serial.println();
}

void setup() {
 Serial.begin(115200);
 pinMode(protocol.rxPin, OUTPUT);
}

void loop() {
  collect(30,1,false ,&protocol);
  transmitData(&protocol);
  sendDebug(&protocol);
  delay(1000);
  collect(30,1,true ,&protocol);
  transmitData(&protocol);
  sendDebug(&protocol);
  delay(1000);
}