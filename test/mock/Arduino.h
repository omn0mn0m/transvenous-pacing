#ifndef ARDUINO_H
#define ARDUINO_H

#include "Arduino.h"

// Mock Objects
int pin_vals[40];

// Mock Functions
int analogRead(int pin) { return pin_vals[pin]; }

// Helper Functions
void set_pin_val(int pin, int val) { pin_vals[pin] = val; }

void delay(int delay_ms) { return; }

#endif // ARDUINO_H