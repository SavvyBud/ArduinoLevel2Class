#include <Arduino.h>

// the maximum pulse we'll listen for - 65 milliseconds is a long time
#define MAXPULSE 65000
#define NUMPULSES 100 
// what our timing resolution should be, larger is better
// as its more 'precise' - but too large and you wont get
// accurate timing

class IRLib{
  
  unsigned int pulses[NUMPULSES][2]; // pair is high and low pulse
  unsigned int currentpulse; // index for pulses we're storing
  int ledPin;
 
  public:
    IRLib(int pin);
    int     listenForIR();
    void    printpulses(void);
    boolean compare(int numpulses, int Signal[]);
    void    sendCode(unsigned int pulses[][2]);
  
  private:
    void   initStoredPulses();
    void   pulseIR(long microsecs);
};
