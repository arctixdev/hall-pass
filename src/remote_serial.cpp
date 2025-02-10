#include "remote_serial.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

RemoteSerial::RemoteSerial() : telnetServer(TELNET_PORT), telnetStarted(false) {}

void RemoteSerial::begin(long baudRate)
{
  Serial.begin(baudRate); // Start USB Serial immediately
  Serial.println("USB Serial Ready.");
}

// Start Telnet after WiFi is connected
void RemoteSerial::startTelnet()
{
  if (WiFi.status() == WL_CONNECTED && !telnetStarted)
  {
    telnetServer.begin();
    telnetServer.setNoDelay(true);
    telnetStarted = true;
    Serial.println("Telnet Remote Serial Started.");
  }
}

// Telnet Task (runs continuously)
void RemoteSerial::serialTask(void *pvParameters)
{
  RemoteSerial *self = (RemoteSerial *)pvParameters;

  while (true)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      self->telnetStarted = false;     // Mark as disconnected
      vTaskDelay(pdMS_TO_TICKS(5000)); // Wait before retrying
      continue;
    }

    if (!self->telnetClient || !self->telnetClient.connected())
    {
      self->telnetClient = self->telnetServer.available();
      if (self->telnetClient)
      {
        Serial.println("New Telnet Client Connected!");
        self->telnetClient.println("ESP32 Remote Serial Ready");
      }
    }

    // Forward Serial to Telnet
    while (Serial.available())
    {
      char c = Serial.read();
      if (self->telnetClient && self->telnetClient.connected())
      {
        self->telnetClient.write(c);
      }
    }

    // Forward Telnet to Serial
    while (self->telnetClient && self->telnetClient.available())
    {
      char c = self->telnetClient.read();
      Serial.write(c);
    }

    vTaskDelay(pdMS_TO_TICKS(10)); // Avoid CPU overuse
  }
}

// Start the Telnet task in a FreeRTOS thread
void RemoteSerial::startTask()
{
  xTaskCreatePinnedToCore(serialTask, "Remote Serial Task", 4096, this, 1, NULL, 1);
}

void RemoteSerial::print(const String &message)
{
  Serial.print(message);
  if (telnetClient && telnetClient.connected())
  {
    telnetClient.print(message);
  }
}

void RemoteSerial::println(const String &message)
{
  Serial.println(message);
  if (telnetClient && telnetClient.connected())
  {
    telnetClient.println(message);
  }
}

void RemoteSerial::printf(const char *format, ...)
{
  char buffer[256];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);

  Serial.print(buffer);
  if (telnetClient && telnetClient.connected())
  {
    telnetClient.print(buffer);
  }
}

// Global instance
RemoteSerial RemoteConsole;
