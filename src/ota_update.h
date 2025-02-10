#ifndef OTA_UPDATE_H
#define OTA_UPDATE_H

#include <Arduino.h>
#include <ArduinoOTA.h>
#include "remote_serial.h"

// Initialize and start OTA
void setupOTA();
void startOTATask();

#endif // OTA_UPDATE_H
