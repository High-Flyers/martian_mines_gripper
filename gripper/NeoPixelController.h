#include <Adafruit_NeoPixel.h>
#include "params.h"

Adafruit_NeoPixel strip(12, LED_PIN, NEO_GRB + NEO_KHZ800);
// LEDY 0,1,2,9,10,11 -> TYL
// LEDY 3,4,5,6,7,8 -> PRZOD

uint16_t hsvb = BLUE_LIGHT;
auto colorb = Adafruit_NeoPixel::ColorHSV(hsvb, 255, 255);

uint16_t hsvr = RED_LIGHT;
auto colorr = Adafruit_NeoPixel::ColorHSV(hsvr, 255, 255);

bool state = 0;  //0 forward, 1 backward

// Ustaw kolor
void colorWipe(uint32_t color, int wait, int start, int end) {
  for(int i = start; i <= end; i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

// Czerwony gradient w gore
void redGradForward(){
  hsvr = hsvr + HSV_BUMP;
  colorr = Adafruit_NeoPixel::ColorHSV(hsvr, 255, 255);
  colorWipe(colorr, CHANGE_TIME, 0, 2); // Red
  colorWipe(colorr, CHANGE_TIME, 9, 11);

  if(hsvr >= 65000){
    hsvr = 0;
  }
}

// Czerwony gradient w dol
void redGradBackward(){
  hsvr = hsvr - HSV_BUMP;
  colorr = Adafruit_NeoPixel::ColorHSV(hsvr, 255, 255);
  colorWipe(colorr, CHANGE_TIME, 0, 2); // Red
  colorWipe(colorr, CHANGE_TIME, 9, 11);

  if(hsvr == 0){
    hsvr = 65000;
  }
}

// Niebieski gradient w gore
void blueGradForward(){
  hsvb = hsvb + HSV_BUMP;
  colorb = Adafruit_NeoPixel::ColorHSV(hsvb, 255, 255);
  colorWipe(colorb, CHANGE_TIME, 3, 8); // Blue
}

// Niebieski gradient w dol
void blueGradBackward(){
  hsvb = hsvb - HSV_BUMP;
  colorb = Adafruit_NeoPixel::ColorHSV(hsvb, 255, 255);
  colorWipe(colorb, CHANGE_TIME, 3, 8); // Blue
}

// Gradient po ringach
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