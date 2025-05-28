#include <ESP32Servo.h>
//#include "ACS712.h"
#include "NeoPixelController.h"
#include "params.h"

Servo s;

//ACS712  ACS(A7, 5.0, 1023, 100);

void setup() {
  Serial.begin(9600);
  pinMode(kranc_L, INPUT_PULLUP);
  pinMode(kranc_R, INPUT_PULLUP);
  pinMode(kranc_B, INPUT_PULLUP); 

  //pinMode(3, OUTPUT);
  pinMode(RX, INPUT_PULLDOWN);
  pinMode(TX, OUTPUT);

  s.attach(serv); 

  //ACS.autoMidPoint();

  strip.begin();           
  strip.show();            
  strip.setBrightness(255); 
}

void loop() {
// Amperomierz
/*
  int mA = ACS.mA_DC();
  Serial.println(mA);
  Serial.println("--------");
*/
  // Komenda zamkniecia lapaczki
  if (digitalRead(RX) == 1){
    // Krec tak dlugo az dojedzie do krancowek
    if (digitalRead(kranc_L) == 1 && digitalRead(kranc_R) == 1) {
      s.writeMicroseconds(forward);
    } 
    // Przy krancowkach zatrzymaj i wyslij info zwrotne do Jetsona
    else {
      s.writeMicroseconds(stop);
      digitalWrite(TX, 1);
    }
  // Komenda otwarcia lapaczki
  } else { //RX ==0
      // Krec tak dlugo az dojedzie do krancowek
      if (digitalRead(kranc_B) == 1) {
        s.writeMicroseconds(backward);
      } 
      // Przy krancowkach zatrzymaj i wyslij info zwrotne do Jetsona
      else {
        s.writeMicroseconds(stop);
        digitalWrite(TX, 0);
      }
  }

  // Animacja LED
  ringGrad();

  // Debug krancowek
  // Serial.println(digitalRead(kranc_L));
  // Serial.println(digitalRead(kranc_R));
  // Serial.println(digitalRead(kranc_B));
  // Debug ledow
  // Serial.println("-----------");
  // Serial.println(hsvr);
  // Serial.println(hsvb);
  // Serial.println(state);
  
  delay(50);
}