#ifndef WIFI_CONNECT_H
#define WIFI_CONNECT_H

#include <Arduino.h>
#include "WiFi.h"
#include <esp_wpa2.h>
#include <esp_wifi.h>
#include "secrets.h"
#include "remote_serial.h"

void connectToWiFi();

#endif // WIFI_CONNECT_H
