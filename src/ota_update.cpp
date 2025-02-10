#include "ota_update.h"
#include "led_control.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// OTA Task Function
void otaTask(void *pvParameters)
{
  while (true)
  {
    ArduinoOTA.handle();
    vTaskDelay(pdMS_TO_TICKS(10)); // Small delay to prevent CPU overload
  }
}

void setupOTA()
{
  RemoteConsole.println("Initializing OTA...");

  ArduinoOTA
      .onStart([]()
               {
            setControllerState(STATE_BOOTING);  // Purple blinking during OTA
            String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
            RemoteConsole.println("OTA Update Start: " + type); })
      .onEnd([]()
             { RemoteConsole.println("\nOTA Update Completed!"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { RemoteConsole.printf("OTA Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
                 RemoteConsole.printf("OTA Error[%u]: ", error);
                 if (error == OTA_AUTH_ERROR)
                   RemoteConsole.println("Auth Failed");
                 else if (error == OTA_BEGIN_ERROR)
                   RemoteConsole.println("Begin Failed");
                 else if (error == OTA_CONNECT_ERROR)
                   RemoteConsole.println("Connect Failed");
                 else if (error == OTA_RECEIVE_ERROR)
                   RemoteConsole.println("Receive Failed");
                 else if (error == OTA_END_ERROR)
                   RemoteConsole.println("End Failed");

                 setControllerState(STATE_ERROR); // Red LED on OTA failure
               });

  ArduinoOTA.begin();
  RemoteConsole.println("OTA Ready!");
}

void startOTATask()
{
  xTaskCreatePinnedToCore(otaTask, "OTA Task", 4096, NULL, 1, NULL, 1);
}
