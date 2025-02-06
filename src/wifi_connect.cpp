#include "led_control.h"
#include "wifi_connect.h"
#include "state_manager.h"

void connectToWiFi()
{
  setControllerState(STATE_BOOTING);
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  esp_wifi_set_storage(WIFI_STORAGE_RAM);

  if (USE_WPA2_ENTERPRISE)
  {
    // === WPA2 Enterprise Connection ===
    Serial.println("Connecting to WPA2 Enterprise Network...");
    Serial.print("SSID: ");
    Serial.println(ENTERPRISE_SSID);

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
    Serial.println("Connecting to Hotspot...");
    Serial.print("SSID: ");
    Serial.println(HOTSPOT_SSID);

    WiFi.begin(HOTSPOT_SSID, HOTSPOT_PASSWORD);
  }

  Serial.println("Connecting...");
  setControllerState(STATE_CONNECTING);
  int maxAttempts = 5; // Maximum number of attempts before timeout
  int attempts = 0;

  while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts)
  {
    Serial.print(".");
    if (WiFi.status() == WL_CONNECT_FAILED)
    {
      Serial.println("\nFailed to connect. Verify credentials.");
      setControllerState(STATE_ERROR);
      break;
    }
    delay(5000);
    attempts++;
  }

  if (attempts >= maxAttempts)
  {
    Serial.println("\nConnection timed out.");
    setControllerState(STATE_ERROR);
  }
  else
  {
    setControllerState(STATE_CONNECTED);
    Serial.println("\nWiFi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }
}
