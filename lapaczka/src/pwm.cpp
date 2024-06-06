#include "pwm.h"

#define PWM_IN 21

volatile uint8_t _pin = PWM_IN;
volatile uint8_t _flag = false;

volatile uint16_t _val = 0;

volatile uint64_t _currentMicros = 0;
volatile uint64_t _pulseStartMicros = 0;

void IRAM_ATTR isrRising(void);
void IRAM_ATTR isrFalling(void);

void Pwm::enable() {
    _flag = false;
    attachInterrupt(_pin, isrRising, RISING);
}

void Pwm::disable() {
    _flag = false;
    detachInterrupt(_pin);
}

bool Pwm::checkTrigger() {
    return _flag;
}

bool Pwm::isAvailable() {
    // Ensuring pwm value is correct and up to date
    if ((micros() - _currentMicros) < 100000 && _val > 950 && _val < 2050) {
        return true;
    } else
        return false;
}

uint16_t Pwm::getUs() {
    return _val;
}

void IRAM_ATTR isrRising(void) {
    // Rising edge. Start counting the pwm frame length
    _currentMicros = micros();
    _pulseStartMicros = _currentMicros;

    // Prepare interupt for detecting the falling edge
    detachInterrupt(_pin);
    attachInterrupt(_pin, isrFalling, FALLING);
}

void IRAM_ATTR isrFalling(void) {
    // Falling edge. Stop counting pwm frame length
    _currentMicros = micros();
    _val = _currentMicros - _pulseStartMicros;

    // Get back to default interrupt configuration
    detachInterrupt(_pin);
    attachInterrupt(_pin, isrRising, RISING);

    if (_val > 1500 && _val < 2100) {  // setting a flag to remember trigger state
        _flag = true;
    }
}
