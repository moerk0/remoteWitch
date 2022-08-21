 
 #ifndef CHASE_H
 #define CHASE_H
 
 #include <Arduino.h>
 #include <RCSwitch.h>




 class ChasePLUGS
 {
 public:
   ChasePLUGS( int numPlugs, unsigned long advanceTime);

   int time2change(byte mode);

    unsigned long advanceTime() const { return _advanceTime; }
    void setAdvanceTime(unsigned long advanceTime) { _advanceTime = advanceTime; }
   //if u run in anticlkw mode, setSnake will also be reveresed
    void setSnake(int seqLeng){ snake = seqLeng ; }
    uint8_t previousPlug(int n);
    void setAdvanceTime(int t){_advanceTime = t;}
         
 
 private:
     int _numPlugs;
     unsigned int _currentIndex;
     unsigned long _advanceTime;
     unsigned long _lastChange;
     int snake;
 };
 
 #endif