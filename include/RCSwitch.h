#ifndef RCSWICH_H
#define RCSWICH_H
#define PHRASE_LENGHT   25
#define PULSE_LENGHT    318  

#define ADDR_1 30
#define ADDR_2 29
#define ADDR_3 27
#define TASTE_A 1
#define TASTE_B 2

#include <Arduino.h>

class RCSwitch
{
    private:

        //TransmitterPin
        const uint8_t txPin;

        //address of specific Plug
        const uint8_t address;
        const uint8_t taste;

        /**One Short pulse HI + One Long Pulse LO = LO
         * One Long pulse HI + One Short Pulse LO = HI
        **/
        const uint16_t pulse_lenght_short;
        const uint16_t pulse_lenght_long ;
        
        // times phrase is sent 
        uint8_t repitions;

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
         * taste has to be a specific input address: 1,2,4,8
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
        uint8_t nPos;

        bool state;


        
        void sendHIGH();
        void sendLOW();


        void collectPhrase();
        void transmitPhrase();


    public:
        RCSwitch(uint8_t, uint8_t,uint8_t);


        void send2Monitor();
        void switchON();
        void switchOFF();
        void switchChange();
        
        void setRepetitions(uint8_t rep){repitions = rep;}
       
        bool getState(){return state;}

};






#endif
