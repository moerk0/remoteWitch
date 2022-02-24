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

void collect(int adrr, int taste, int state ,struct Protocol *p){
  p->nPos = 1;  //  nPos = 0, always startBit 0
  
  //Reset Array
  for (int i = 0; i < PROTOCOL_LENGTH; i++)
  {
    p->arr[i] = 0;
  }
  
  //write address bits
  for (int i = 0; i < 5; i++) {
    if (adrr & (1 << i)) {
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
      p->arr[p->nPos++] = 1;
      p->nPos++;
    }
    else {
      p->nPos += 2;
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

      if ( !p->arr[i] ){
        sendLOW(&protocol);
      }

      else{
        sendHIGH(&protocol);
      }
    delay(10);
    }
  }  
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
}

void loop() {
  collect(1,1,false ,&protocol);
  transmitData(&protocol);
  sendDebug(&protocol);
  delay(1000);
  collect(1,1,true ,&protocol);
  transmitData(&protocol);
  sendDebug(&protocol);
  delay(1000);
}