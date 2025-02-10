#ifndef REMOTE_SERIAL_H
#define REMOTE_SERIAL_H

#include <Arduino.h>
#include <WiFiServer.h>
#include <WiFi.h>

#define TELNET_PORT 23 // Standard Telnet Port

class RemoteSerial
{
public:
  RemoteSerial();
  void begin(long baudRate); // Start USB Serial
  void startTelnet();        // Start Telnet after WiFi is connected
  void print(const String &message);
  void println(const String &message);
  void printf(const char *format, ...);
  void startTask();                           // Starts FreeRTOS Task for Telnet
  static void serialTask(void *pvParameters); // Task function

private:
  WiFiServer telnetServer;
  WiFiClient telnetClient;
  bool telnetStarted;
};

extern RemoteSerial RemoteConsole; // Global instance

#endif // REMOTE_SERIAL_H
