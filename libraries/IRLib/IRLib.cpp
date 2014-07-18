
#include "IRLib.h"
#define IRpin_PIN      PIND
#define FUZZINESS 20
#define RESOLUTION 20

IRLib::IRLib(int pin){
  ledPin = pin;
  currentpulse = 0;
  pinMode(ledPin, INPUT);
}

// This procedure sends a 38KHz pulse to the IRledPin 
// for a certain # of microseconds. We'll use this whenever we need to send codes
void IRLib::pulseIR(long microsecs) {
  // we'll count down from the number of microseconds we are told to wait
 
  cli();  // this turns off any background interrupts
 
  while (microsecs > 0) {
    // 38 kHz is about 13 microseconds high and 13 microseconds low
   digitalWrite(ledPin, HIGH);  // this takes about 3 microseconds to happen
   delayMicroseconds(10);         // hang out for 10 microseconds, you can also change this to 9 if its not working
   digitalWrite(ledPin, LOW);   // this also takes about 3 microseconds
   delayMicroseconds(10);         // hang out for 10 microseconds, you can also change this to 9 if its not working
 
   // so 26 microseconds altogether
   microsecs -= 26;
  }
 
  sei();  // this turns them back on
}
 
 
void IRLib::printpulses(void) {
  Serial.println("\n\r\n\rReceived: \n\rOFF \tON");
  for (int i = 0; i < currentpulse; i++) {
    Serial.print(pulses[i][0] * RESOLUTION, DEC);
    Serial.print(" usec, ");
    Serial.print(pulses[i][1] * RESOLUTION, DEC);
    Serial.println(" usec");
  }
 
  // print it in a 'array' format
  Serial.println("int IRsignal[] = {");
  Serial.println("// ON, OFF (in 10's of microseconds)");
  for (int i = 0; i < currentpulse-1; i++) {
    Serial.print("\t"); // tab
    Serial.print(pulses[i][1] * RESOLUTION / 10, DEC);
    Serial.print(", ");
    Serial.print(pulses[i+1][0] * RESOLUTION / 10, DEC);
    Serial.println(",");
  }
  Serial.print("\t"); // tab
  Serial.print(pulses[currentpulse-1][1] * RESOLUTION / 10, DEC);
  Serial.print(", 0}; // total=");
  Serial.println(currentpulse);
}

void IRLib::initStoredPulses(){
  for(int i=0;i<NUMPULSES;i++){
    pulses[i][0] = 0;
    pulses[i][1] = 0;
  }
}

int IRLib::listenForIR(void) {
  currentpulse = 0;
  initStoredPulses();
  while (1) {
    unsigned int highpulse, lowpulse;  // temporary storage timing
    highpulse = lowpulse = 0; // start out with no pulse length
 
//  while (digitalRead(IRpin)) { // this is too slow!
    while (IRpin_PIN & (1 << ledPin)) {
       // pin is still HIGH
 
       // count off another few microseconds
       highpulse++;
       delayMicroseconds(RESOLUTION);
 
       // If the pulse is too long, we 'timed out' - either nothing
       // was received or the code is finished, so print what
       // we've grabbed so far, and then reset
       if ((highpulse >= MAXPULSE) && (currentpulse != 0)) {
         return currentpulse;
       }
    }
    // we didn't time out so lets stash the reading
    pulses[currentpulse][0] = highpulse;
 
    // same as above
    while (! (IRpin_PIN & _BV(ledPin))) {
       // pin is still LOW
       lowpulse++;
       delayMicroseconds(RESOLUTION);
       if ((lowpulse >= MAXPULSE)  && (currentpulse != 0)) {
         return currentpulse;
       }
    }
    pulses[currentpulse][1] = lowpulse;
 
    // we read one high-low pulse successfully, continue!
    currentpulse++;
  }
}


boolean IRLib::compare(int numpulses, int Signal[]) {
 
  int errorCount = 0;
#ifdef DEBUG
  Serial.println("  ORIG        HEARD");  
#endif
  for (int i=0; i< numpulses-1; i++) {
    int oncode = pulses[i][1] * RESOLUTION / 10;
    int offcode = pulses[i+1][0] * RESOLUTION / 10 ;
 
#ifdef DEBUG
    Serial.print(i+1);
    Serial.print(". ");
    Serial.print(Signal[i*2 + 0]); // the ON signal 
    Serial.print(" - ");
    Serial.print(Signal[i*2 + 1]); // the OFF signal 

    Serial.print("  \t"); // tab
    Serial.print(oncode); // the OFF signal we want 
    Serial.print(" - ");
    Serial.print(offcode); // the OFF signal we heard    
#endif
 
    // check to make sure the error is less than FUZZINESS percent
    if ( abs(oncode - Signal[i*2 + 0]) <= (Signal[i*2 + 0] * FUZZINESS / 100)) {
#ifdef DEBUG
      Serial.print(" (ok)");
#endif      
    } else {
#ifdef DEBUG      
      Serial.print(" (x)");
#endif      
      // we didn't match perfectly, return a false match
      errorCount = errorCount + 1;
      //return false;
    }
 
#ifdef DEBUG    
#endif    
 
    if ( abs(offcode - Signal[i*2 + 1]) <= (Signal[i*2 + 1] * FUZZINESS / 100)) {
#ifdef DEBUG      
      Serial.print(" (ok)");
#endif      
    } else {
#ifdef DEBUG      
      Serial.print(" (x)");
#endif      
      errorCount = errorCount + 1;
      // we didn't match perfectly, return a false match
      //return false;
    }
#ifdef DEBUG 
    Serial.println();
#endif
    if(errorCount > 5)
      return false;
  }
  // Everything matched!
  return true;
}

