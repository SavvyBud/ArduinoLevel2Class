#include <RTTTLib.h>

#define AMAZING_GRACE "Amazing Grace:d=16,o=5,b=80:8c,2f,a,g,f,2a,8a,8g,2f,4d,2c,8c,2f,a,g,f,2a,8g,8a,2c6."
#define ALVIN "Batman:d=8,o=5,b=180:d,d,c#,c#,c,c,c#,c#,d,d,c#,c#,c,c,c#,c#,d,d#,c,c#,c,c,c#,c#,f,p,4f"

char *tunes[] = {STAR_WARS,GOOD_BAD,TOPGUN,MISSION_IMP,AMAZING_GRACE,ALVIN};

int BUZZER_PIN = 3;
int pushButton = 2;

int i =0;
RTTTLib rt(BUZZER_PIN);

void setup(){
  pinMode(pushButton,INPUT);
}

void loop(){
  if(i>5)
    i=0;
  rt.play(tunes[i]);
  i++;
  
  int buttonState = LOW;
  while(buttonState == LOW) {
    buttonState = digitalRead(pushButton);
    delay(5);
  }
}


