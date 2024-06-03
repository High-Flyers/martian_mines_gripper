#include <Servo.h>
#include "ACS712.h"
#include <Adafruit_NeoPixel.h>

#define backward 1000
#define forward 2000
#define stop 1500

#define RX 19
#define TX 18

#define kranc_L 2
#define kranc_R 5
#define kranc_B 8

#define serv 9

#define LED_PIN 14

Servo s;

ACS712  ACS(A7, 5.0, 1023, 100);

Adafruit_NeoPixel strip(12, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  pinMode(kranc_L, INPUT);
  pinMode(kranc_R, INPUT);
  pinMode(kranc_B, INPUT); 

  pinMode(3, OUTPUT);
  pinMode(19, INPUT);
  pinMode(18, OUTPUT);

  s.attach(serv); 

  ACS.autoMidPoint();

  strip.begin();           
  strip.show();            
  strip.setBrightness(50); 
}

void loop() {

  int mA = ACS.mA_DC();
  Serial.println(mA);
  Serial.println("--------");
  
  if(digitalRead(RX) == 1){
    if(digitalRead(kranc_L) == 1 && digitalRead(kranc_R) == 1) {
      s.writeMicroseconds(forward);
    } else{
      s.writeMicroseconds(stop);
      digitalWrite(TX, 1);
    }

  } else{ //RX ==0
      if(digitalRead(kranc_B) == 1) {
        s.writeMicroseconds(backward);
      } else{
        s.writeMicroseconds(stop);
        digitalWrite(TX, 0);
      }
  }
  
  Serial.println(digitalRead(kranc_L));
  Serial.println(digitalRead(kranc_R));
  Serial.println(digitalRead(kranc_B));
  colorWipe(strip.Color(  0,   0, 255), 100, 3, 8); // Blue
  colorWipe(strip.Color(  255, 0, 0), 100, 0, 2); // Red
  colorWipe(strip.Color(  255, 0, 0), 100, 9, 11);
  delay(100);
}

void colorWipe(uint32_t color, int wait, int start, int end) {
  for(int i = start; i <= end; i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}