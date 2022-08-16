#ifndef RCSWICH_H
#define RCSWICH_H
#define PHRASE_LENGHT   25
#define PULSE_LENGHT    320
#include <Arduino.h>

class RCSwitch
{
    private:

        //TransmitterPin
        int txPin;

        //Adress of specific Plug
        int address;
        int taste;

        /**One Short pulse HI + One Long Pulse LO = LO
         * One Long pulse HI + One Short Pulse LO = HI
        **/
        int pulse_lenght_short;
        int pulse_lenght_long ;
        
        // times phrase is sent 
        int repitions;

        /**the actual phrase being sent. it looks like this:
         *          
         *  sync |   Address         taste       unused   state
         *  0   |  10 10 10 10 10 | 10 10 10 10 | 10 | 10 10 = 25 bits
         * 
         * as int: 30 29 27 23 15 | 1  2  4  8         F  T
         * either of these values need to be fed to SwOn() and Off()
         * 
         * sync is always LO
         * 
         * Address is a value between 0 & 31, it has to match the dip switches
         * on the physical RC plug. Multiple plugs can have the same Addr
         * 
         * taste has to be a specific input num: 1,2,4,8
         * 
         * unuesed is usually refered to the default values 10
         * 
         * state can be either 10 00 for LO, or 00 10 for HI
         * 
         * Only every second bits differ. in the example above they are marked as 1
         * 
         * if addr. or taste is selected the corresponding bit is set to LO
         * 
         * 
         * Example input:
         * addr = 29, taste = 0, status = HI
         * output:
         *  0   |  10 00 10 10 10 | 00 10 10 10 | 10 | 00 10
        **/
        int phrase[PHRASE_LENGHT] = {0};
        int nPos;

        bool state;
        int delayT;
        unsigned long lastChange;

        
        void sendHIGH();
        void sendLOW();


        void collectPhrase();
        void transmitPhrase();
        void send2Monitor();


    public:
        RCSwitch(uint8_t, uint8_t,uint8_t);


        void switchON();
        void switchOFF();
        void switchChange(uint8_t adrr, uint8_t taste);
        void setRepetitions(int rep){repitions = rep;}
        void setDelayT(int d){d = delayT;}
        void setOffset(int o){o = lastChange;}
};






#endif
