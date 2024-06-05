#include <ESP32Servo.h>
//#include "ACS712.h"
#include <Adafruit_NeoPixel.h>

#define backward 1000
#define forward 2000
#define stop 1510

#define RX 21 //SDA
#define TX 22 //SCL

#define kranc_L 25
#define kranc_R 26
#define kranc_B 32

#define serv 33

#define LED_PIN 27

#define HSV_BUMP 3000
#define CHANGE_TIME 10
#define BLUE_LIGHT 25000
#define BLUE_DARK 45000
#define RED_LIGHT 50000
#define RED_DARK 5000

Servo s;

//ACS712  ACS(A7, 5.0, 1023, 100);

Adafruit_NeoPixel strip(12, LED_PIN, NEO_GRB + NEO_KHZ800);

uint16_t hsvb = BLUE_LIGHT;
auto colorb = Adafruit_NeoPixel::ColorHSV(hsvb, 255, 255);

uint16_t hsvr = RED_LIGHT;
auto colorr = Adafruit_NeoPixel::ColorHSV(hsvr, 255, 255);

bool state = 0;  //0 forward, 1 backward

void setup() {
  Serial.begin(9600);
  pinMode(kranc_L, INPUT_PULLUP);
  pinMode(kranc_R, INPUT_PULLUP);
  pinMode(kranc_B, INPUT_PULLUP); 

  pinMode(3, OUTPUT);
  pinMode(19, INPUT);
  pinMode(18, OUTPUT);

  s.attach(serv); 

  //ACS.autoMidPoint();

  strip.begin();           
  strip.show();            
  strip.setBrightness(255); 
}

void loop() {
/*
  int mA = ACS.mA_DC();
  Serial.println(mA);
  Serial.println("--------");
*/
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

  ringGrad();
  Serial.println("-----------");
  Serial.println(hsvr);
  Serial.println(hsvb);
  Serial.println(state);
  delay(50);
}

void colorWipe(uint32_t color, int wait, int start, int end) {
  for(int i = start; i <= end; i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void redGradForward(){

  hsvr = hsvr + HSV_BUMP;
  colorr = Adafruit_NeoPixel::ColorHSV(hsvr, 255, 255);
  colorWipe(colorr, CHANGE_TIME, 0, 2); // Red
  colorWipe(colorr, CHANGE_TIME, 9, 11);

  if(hsvr >= 65000){
    hsvr = 0;
  }
}

void redGradBackward(){
  hsvr = hsvr - HSV_BUMP;
  colorr = Adafruit_NeoPixel::ColorHSV(hsvr, 255, 255);
  colorWipe(colorr, CHANGE_TIME, 0, 2); // Red
  colorWipe(colorr, CHANGE_TIME, 9, 11);

  if(hsvr == 0){
    hsvr = 65000;
  }
}

void blueGradForward(){
  hsvb = hsvb + HSV_BUMP;
  colorb = Adafruit_NeoPixel::ColorHSV(hsvb, 255, 255);
  colorWipe(colorb, CHANGE_TIME, 3, 8); // Blue
}

void blueGradBackward(){
  hsvb = hsvb - HSV_BUMP;
  colorb = Adafruit_NeoPixel::ColorHSV(hsvb, 255, 255);
  colorWipe(colorb, CHANGE_TIME, 3, 8); // Blue
}

void ringGrad(){
  switch(state){
    case 0:
      redGradForward();
      blueGradForward();
      if(hsvr >= RED_DARK && hsvr < RED_LIGHT && hsvb >= BLUE_DARK)
        state = !state;
    break;

    case 1:
      redGradBackward();
      blueGradBackward();
      if(hsvr <= RED_LIGHT && hsvb <= BLUE_LIGHT)
        state = !state;
    break;
  }

  strip.show(); 
}
