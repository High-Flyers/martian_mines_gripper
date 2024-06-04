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
#define MAXIMUM_BRIGHTNESS 255
Servo s;

ACS712  ACS(A7, 5.0, 1023, 100);

Adafruit_NeoPixel strip(12, LED_PIN, NEO_GRB + NEO_KHZ800);

int maxBrightness = 255;
int minBrightness = 0;

uint16_t hsvb = 25000;
auto colorb = Adafruit_NeoPixel::ColorHSV(hsvb, 255, 255);

uint16_t hsvr = 55000;
auto colorr = Adafruit_NeoPixel::ColorHSV(hsvr, 255, 255);

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

  redgrad();
  bluegrad();

  delay(100);
}

void colorWipe(uint32_t color, int wait, int start, int end) {
  for(int i = start; i <= end; i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void redgrad(){
  
  hsvr = hsvr + 1000;
  if(hsvr > 10000)
    hsvr = 55000;
  colorr = Adafruit_NeoPixel::ColorHSV(hsvr, 255, 255);
  colorWipe(colorr, 100, 0, 2); // Red
  colorWipe(colorr, 100, 9, 11);
  if(hsvr >= 65000){
    hsvr = 0;
  }
}

void bluegrad(){
  hsvb = hsvb + 1000;
  if(hsvb > 45000){
    hsvb = 25000;
  }
  colorb = Adafruit_NeoPixel::ColorHSV(hsvb, 255, 255);
  colorWipe(colorb, 100, 3, 8); // Blue
}

/*
void ring(){

  strip.setPixelColor(0, colorr1);
  strip.setPixelColor(1, colorr2);
  strip.setPixelColor(2, colorr3);

  strip.setPixelColor(3, colorb1);
  strip.setPixelColor(4, colorb2);
  strip.setPixelColor(5, colorb3);

  strip.setPixelColor(6, colorb1);
  strip.setPixelColor(7, colorb2);
  strip.setPixelColor(8, colorb3);

  strip.setPixelColor(9, colorr1);
  strip.setPixelColor(10, colorr2);
  strip.setPixelColor(11, colorr3);

  strip.show(); 

}
*/