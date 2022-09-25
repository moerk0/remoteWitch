 
#include "chase.h"
#include <Arduino.h>



 
ChasePLUGS::ChasePLUGS(int numPlugs, unsigned long advanceTime)
    : _numPlugs(numPlugs)
    , _currentIndex(0)
    , _defaultTime(advanceTime)
    , _lastChange(0)
    , _tries(0)
    {
        _advanceTime = _defaultTime;
    }

int ChasePLUGS::getNextPlug(byte mode){
    _mode = mode;
    
    if ((millis() - _lastChange) >= _advanceTime) {
        _prevIndex = _currentIndex;
        
        switch (mode){
            case chase:
            _currentIndex++ ;
            _currentIndex %= _numPlugs;
            break;

            case randomIndexChase:
            _currentIndex = random(0,_numPlugs);
            break;

        }
        _lastChange = millis();
        return _currentIndex;
    }
    else{return -1;}
}


 

 uint8_t ChasePLUGS::previousPlug(int n){
    if (_mode == chase)
    {
    int result;
    result =(_currentIndex + _numPlugs - n ) % _numPlugs; 
    (result == -1) ? result = _numPlugs - 1 : result = result;
    return result;    
    }
    else if(_mode == randomIndexChase){
        return _prevIndex;
    }
    else{
        return 0;
    }

 }

void ChasePLUGS::IdleIntervalHandler(int firstT, int secondT, int changeAfterNTries){
    
   
        if(_tries <= changeAfterNTries){
            this->_advanceTime = firstT;
            _tries++;
        }
        else this->_advanceTime = secondT;
    #if PLUG_MSG == true
        Serial.print("Change of intervals in ");
        Serial.println(changeAfterNTries - _tries);
    #endif
    }

void ChasePLUGS::debugMsg(){
}
 