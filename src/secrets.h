#ifndef SECRETS_H
#define SECRETS_H

// === Toggle between WPA2 Enterprise and Hotspot ===
#define USE_WPA2_ENTERPRISE false // Set to `false` to use phone hotspot

// WPA2 Enterprise Credentials (School WiFi)
extern const char *ENTERPRISE_SSID;
extern const char *EAP_IDENTITY;
extern const char *EAP_USERNAME;
extern const char *EAP_PASSWORD;

// WPA2-PSK (Normal WiFi, e.g., phone hotspot)
extern const char *HOTSPOT_SSID;
extern const char *HOTSPOT_PASSWORD;

#endif // SECRETS_H
