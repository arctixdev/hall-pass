#include <Arduino.h>
#include "state_manager.h"
#include "led_control.h"
#include "wifi_connect.h"
#include <thread.hpp>
#include "remote_serial.h"

using namespace freertos;

thread worker;

void setup()
{
  RemoteConsole.begin(115200); // Start USB Serial immediately
  // while (!Serial)
    // ;
  RemoteConsole.println("Starting...");

  initStateManager();
  setupLEDs();
  startLEDTask();
  connectToWiFi();

  worker = thread::create_affinity(1 - thread::current().affinity(), [](void *)
                                   {
        while(true) {
          RemoteConsole.println("Hello from worker thread");
          delay(750);
        } }, nullptr, 1, 2000);

  worker.start();
}

void loop()
{
  // Simulate a warning after 10 seconds
  delay(5000);
  setControllerState(STATE_WARNING);

  // After another 10 seconds, set a custom pattern
  delay(5000);
  setCustomPattern(CRGB::DarkGreen, 500, 100, true); // Fast blinking Cyan
}
