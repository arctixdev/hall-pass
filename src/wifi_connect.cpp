#include "led_control.h"
#include "wifi_connect.h"
#include "state_manager.h"
#include "ota_update.h"
#include "mdns_service.h"

void connectToWiFi()
{
  setControllerState(STATE_BOOTING);
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  esp_wifi_set_storage(WIFI_STORAGE_RAM);

  if (USE_WPA2_ENTERPRISE)
  {
    // === WPA2 Enterprise Connection ===
    RemoteConsole.println("Connecting to WPA2 Enterprise Network...");
    RemoteConsole.print("SSID: ");
    RemoteConsole.println(ENTERPRISE_SSID);

    WiFi.begin(ENTERPRISE_SSID);
    esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
    esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_USERNAME, strlen(EAP_USERNAME));
    esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));
    esp_wifi_sta_wpa2_ent_enable();
    WiFi.begin(ENTERPRISE_SSID);
  }
  else
  {
    // === WPA2-PSK (Phone Hotspot) Connection ===
    RemoteConsole.println("Connecting to Hotspot...");
    RemoteConsole.print("SSID: ");
    RemoteConsole.println(HOTSPOT_SSID);

    WiFi.begin(HOTSPOT_SSID, HOTSPOT_PASSWORD);
  }

  RemoteConsole.println("Connecting...");
  setControllerState(STATE_CONNECTING);
  int maxAttempts = 5; // Maximum number of attempts before timeout
  int attempts = 0;

  while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts)
  {
    RemoteConsole.print(".");
    if (WiFi.status() == WL_CONNECT_FAILED)
    {
      RemoteConsole.println("\nFailed to connect. Verify credentials.");
      setControllerState(STATE_ERROR);
      break;
    }
    delay(5000);
    attempts++;
  }

  if (attempts >= maxAttempts)
  {
    RemoteConsole.println("\nConnection timed out.");
    setControllerState(STATE_ERROR);
  }
  else
  {
    setControllerState(STATE_CONNECTED);
    RemoteConsole.println("\nWiFi connected!");
    RemoteConsole.print("IP Address: ");
    RemoteConsole.println(WiFi.localIP().toString());
    setupOTA();
    startOTATask();
    RemoteConsole.startTelnet();
    RemoteConsole.startTask();
    setupMDNS();
  }
}
