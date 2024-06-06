#pragma once

#include <Arduino.h>

namespace Pwm {
void enable();
void disable();

bool checkTrigger();

uint16_t getUs();
bool isAvailable();
}  // namespace Pwm