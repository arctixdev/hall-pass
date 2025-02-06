#include <Arduino.h>
#include "wifi_connect.h"
#include "led_control.h"
#include <thread.hpp>

using namespace freertos;

thread worker;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ; // Wait for USB CDC Serial
  Serial.println("Starting...");

  setupLEDs();     // Initialize LEDs
  connectToWiFi(); // Connect to WiFi

  worker = thread::create_affinity(1 - thread::current().affinity(), [](void *)
                                   {
        while(true) {
            Serial.println("Hello from worker thread");
            delay(500);
        } }, nullptr, 1, 2000);

  worker.start();
}

void loop()
{
  setLEDColor(255, 0, 0); // Red
  Serial.println("RED LED ON");
  delay(1000);

  setLEDColor(0, 255, 0); // Green
  Serial.println("GREEN LED ON");
  delay(1000);

  setLEDColor(0, 0, 255); // Blue
  Serial.println("BLUE LED ON");
  delay(1000);
}
