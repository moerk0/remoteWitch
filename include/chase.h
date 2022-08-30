 
 #ifndef CHASE_H
 #define CHASE_H
 
 #include <Arduino.h>


enum Mode{
    chase,
    randomIndexChase,
    randomIntervalChase,
};


 class ChasePLUGS
 {
 public:
   ChasePLUGS(int numPlugs, unsigned long advanceTime);

    int getNextPlug(byte mode);
    int getPrevIndex(){return _prevIndex;}

    void IdleIntervalHandler(int firstT, int secondT, int changeAfterNTries);
    unsigned long advanceTime() { return _advanceTime; }
    void setAdvanceTime(unsigned long advanceTime) { _advanceTime = advanceTime; }
    void setSnake(int seqLeng){ snake = seqLeng ; }
    uint8_t previousPlug(int n);
    //void setAdvanceTime(int t){_advanceTime = t;}
         
 
 private:
    int _numPlugs;
    unsigned int _currentIndex;
    unsigned long _defaultTime;
    unsigned long _lastChange;

    unsigned int _prevIndex;
    unsigned long _advanceTime;
    uint8_t _mode;
    int snake;
 };
 
 #endif