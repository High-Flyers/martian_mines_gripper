#include <Arduino.h>
#include "pwm.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Pwm::enable();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("dupa");
  Serial.println(Pwm::getUs());
  delay(1000);
}
