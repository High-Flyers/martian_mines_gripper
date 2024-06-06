#include<ESP32Servo.h>
#include <Arduino.h>
#include "pwm.h"
#include <Adafruit_NeoPixel.h>

#define kranc_L 25
#define kranc_R 26
#define kranc_B 32

#define serv 33

#define backward 1000
#define forward 2000
#define stop 1500

#define LED_PIN 27

#define HSV_BUMP 3000
#define CHANGE_TIME 10
#define BLUE_LIGHT 25000
#define BLUE_DARK 45000
#define RED_LIGHT 50000
#define RED_DARK 5000

Adafruit_NeoPixel strip(12, LED_PIN, NEO_GRB + NEO_KHZ800);

uint16_t hsvb = BLUE_LIGHT;
auto colorb = Adafruit_NeoPixel::ColorHSV(hsvb, 255, 255);

uint16_t hsvr = RED_LIGHT;
auto colorr = Adafruit_NeoPixel::ColorHSV(hsvr, 255, 255);

Servo s;

bool open = 0;

uint16_t state = 1500;

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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(kranc_L, INPUT_PULLUP);
  pinMode(kranc_R, INPUT_PULLUP);
  pinMode(kranc_B, INPUT_PULLUP);

  s.attach(serv); 

  strip.begin();           
  strip.show();            
  strip.setBrightness(255); 

  Pwm::enable();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("dupa");
  Serial.println(Pwm::getUs());
  state = Pwm::getUs();
  /*
   if (Pwm::isAvailable() && Pwm::checkTrigger()) {
        Pwm::disable();
        open = !open;
        //delay(3000);
        Pwm::enable();
  }
*/
    
  if(state < 1200 && state > 800){
    if(digitalRead(kranc_L) == 1 && digitalRead(kranc_R) == 1) {
      s.writeMicroseconds(forward);
    } else{
      s.writeMicroseconds(stop);
    }

  } else if (state > 1800 && state < 2100){ //RX ==0
      if(digitalRead(kranc_B) == 1) {
        s.writeMicroseconds(backward);
      } else{
        s.writeMicroseconds(stop);
      }
  } else {
    s.writeMicroseconds(stop);
  }
  
  //Serial.println(open); //GOWNO
  ringGrad();
  delay(50);
}
