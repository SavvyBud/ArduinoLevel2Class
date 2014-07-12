
float major[] = {440, 493.9, 523.3,587.3,659.3,698.5,784.0,880} ;

void setup(){
  pinMode(11,OUTPUT);
}

void loop(){
  for (int i=0;i<8;i++) {
    tone(11,major[i]);
    delay(250);
  }
  for(int i=7;i>=0;i--){
    tone(11,major[i]);
    delay(250);
  }
}

