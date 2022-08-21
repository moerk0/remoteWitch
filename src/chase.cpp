 
#include "chase.h"
#include <Arduino.h>
#include <RCSwitch.h>

enum Mode{
    clockwise,
    anticlockwise,
};
 
ChasePLUGS::ChasePLUGS(int numPlugs, unsigned long advanceTime)
    : _numPlugs(numPlugs)
    , _currentIndex(0)
    , _advanceTime(advanceTime)
    , _lastChange(0)
{

}

int ChasePLUGS::time2change(byte mode){


    if ((millis() - _lastChange) >= _advanceTime) {
        // switch (mode){
        //     case clockwise:
                
                _currentIndex++ ;
                _currentIndex %= _numPlugs;
            // break;

            // case anticlockwise:
            //     advance(previousPin(snake), _pins[_currentIndex], mode);
            //     if(_currentIndex == 0)
            //         _currentIndex = _numPlugs - 1;
            //     else
            //     _currentIndex--;
            // break;
        

        _lastChange = millis();
        return _currentIndex;
    }
    else{return -1;}
}


 

 uint8_t ChasePLUGS::previousPlug(int n){
     
    int result;
    result =(_currentIndex + _numPlugs - n) % _numPlugs; 
    (result == -1) ? result = _numPlugs - 1 : result = result;
    return result;

 }