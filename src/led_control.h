#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <Arduino.h>
#include <FastLED.h>
#include "state_manager.h"

#define NUM_LEDS 1
#define DATA_PIN 48

// Pattern structure to define LED behavior
struct LedPattern
{
  CRGB color;
  int onTime;  // Time LED is ON (ms)
  int offTime; // Time LED is OFF (ms)
  bool isBlinking;
};

// Function declarations
void setupLEDs();
void startLEDTask();
void setCustomPattern(CRGB color, int onTime, int offTime, bool isBlinking);

#endif // LED_CONTROL_H
