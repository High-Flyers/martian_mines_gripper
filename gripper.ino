#include <Servo.h>
#include "ACS712.h"

#define backward 1000
#define forward 2000
#define stop 1500

#define RX 19
#define TX 18

#define kranc_L 2
#define kranc_R 5
#define kranc_B 8

#define serv 9

Servo s;

ACS712  ACS(A7, 5.0, 1023, 100);

void setup() {
  Serial.begin(9600);
  pinMode(kranc_L, INPUT_PULLUP);
  pinMode(kranc_P, INPUT_PULLUP);
  pinMode(kranc_B, INPUT_PULLUP); 

  pinMode(3, OUTPUT);
  pinMode(19, INPUT);
  pinMode(18, OUTPUT);

  s.attach(serv); 

  ACS.autoMidPoint();
}

void loop() {

  int mA = ACS.mA_DC();
  Serial.println(mA);
  Serial.println("--------");

  if(digitalRead(RX) == 1){
    while(digitalRead(kranc_L) == 1 && digitalRead(kranc_P) == 1) {
      s.writeMicroseconds(forward);
    }
    s.writeMicroseconds(stop);
    digitalWrite(TX, 1);

  } else{
    while(digitalRead(kranc_B) == 1) {
      s.writeMicroseconds(backward);
    }
    s.writeMicroseconds(stop);
    digitalWrite(TX, 0);
  }
  delay(100);
}