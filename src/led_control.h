#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 1
#define DATA_PIN 48

void setupLEDs();
void setLEDColor(uint8_t r, uint8_t g, uint8_t b);

#endif // LED_CONTROL_H
