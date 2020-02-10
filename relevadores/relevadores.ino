#include <LiquidCrystal.h>

LiquidCrystal lcd( 5, 6, 9, 10, 11, 12 ); 

class Relevadores 
{
  uint8_t relPin;
  bool relEst;
  long ontime;
  long offtime;
  unsigned long prevMillis;

public:
  Relevadores (uint8_t pin, long on, long off) {
    relPin = pin;
    pinMode(relPin, OUTPUT);
    ontime = on;
    offtime = off;
    relEst = LOW;
    prevMillis = 0;

  };
  void actualRel() {
    unsigned long actualMillis = millis();
    if((relEst == HIGH) && (actualMillis - prevMillis >= ontime))
    {
      relEst = LOW;
      prevMillis = actualMillis;
      digitalWrite(relPin, relEst);
      Serial.print("Relevador apagado "); Serial.println(relPin);
      lcd.clear(); lcd.home(); lcd.print("Relevador off "); lcd.print(relPin);
    } else if ((relEst==LOW) && (actualMillis - prevMillis >= offtime)) {
      relEst = HIGH;
      prevMillis = actualMillis;
      digitalWrite(relPin, relEst);
      Serial.print("Relevador prendido "); Serial.println(relPin);
      lcd.clear(); lcd.home(); lcd.print("Relevador on "); lcd.print(relPin);
    }
  }

  bool edoRel() {
    return relEst;
  }
};

Relevadores relHum(A2, 1200, 3000);
Relevadores relTem(A3, 5000, 5200);
Relevadores relLuz(A4, 3500, 7300);
Relevadores relLl(A5, 4000, 2500);

void setup() {
  Serial.begin(9600);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  lcd.begin(16,2);
}

void loop() {
  relHum.actualRel();
  relTem.actualRel();
  relLuz.actualRel();
  relLl.actualRel();
}
