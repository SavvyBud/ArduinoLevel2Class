#include <DHT.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
// LiquidCrystal_I2C lcd(0x20, 4, 5, 6, 0, 1, 2, 3, 7, NEGATIVE);
 // Addr, En, Rw, Rs, d4, d5, d6, d7, backlighpin, polarity 
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

#define DHTPIN 13     // what pin we're connected to
#define DHTTYPE DHT11   

DHT dht(DHTPIN, DHTTYPE);


// This is a function to convert the celsius reading into fahrenheit
float fahrenheit(float t){
   float f = 1.8*t+32; 
   return f;  
} 

void setup()
{
    Serial.begin(9600);
    lcd.begin(16,2);
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Temp & Humidity");
    lcd.setCursor(0, 1);
    lcd.print("By SavvyBud");
    delay(3000);
}

void loop()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  // check if returns are valid, if 
  //they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
      Serial.println("Failed to read from DHT");
  } else {
      // convert the celsius into Fahranheit
      float f = fahrenheit(t);
      lcd.setCursor(0,0);
      lcd.print("Humidity: ");
      lcd.print((int)h);
      lcd.print(" % ");
      lcd.setCursor(0,1);
      lcd.print("Temp:     ");
      lcd.print((int)f);
      lcd.print(" F");
      
      // Compute heat index
      // Must send in temp in Fahrenheit!
      float hi = dht.computeHeatIndex(f, h);
    
      Serial.print("Humidity: "); 
      Serial.print(h);
      Serial.print(" %\t");
      Serial.print("Temperature: "); 
      Serial.print(t);
      Serial.print(" *C ");
      Serial.print(f);
      Serial.print(" *F\t");
      Serial.print("Heat index: ");
      Serial.print(hi);
      Serial.println(" *F");      
  } 
  delay(2000);
}
