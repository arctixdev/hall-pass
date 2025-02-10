#include "mdns_service.h"
#include "WiFi.h"
#include <ESPmDNS.h>

#define MDNS_HOSTNAME "esp32"

void setupMDNS()
{
  if (!MDNS.begin(MDNS_HOSTNAME))
  {
    RemoteConsole.println("⚠️ Error starting mDNS!");
    return;
  }

  MDNS.addService("http", "tcp", 80);      // Web server discovery
  MDNS.addService("telnet", "tcp", 23);    // Remote Serial discovery
  MDNS.addService("arduino", "tcp", 3232); // OTA discovery

  RemoteConsole.println("✅ mDNS started: You can now access:");
  RemoteConsole.printf("   - http://%s.local\n", MDNS_HOSTNAME);
  RemoteConsole.printf("   - telnet %s.local 23\n", MDNS_HOSTNAME);
  RemoteConsole.printf("   - OTA via esp32.local\n");
}
