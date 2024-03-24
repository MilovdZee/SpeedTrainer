void setup_wifi() {
  Settings settings = get_settings();
  if (settings.ssid[0] != 0) {
    Serial.printf("Connecting to WiFi '%s': .", settings.ssid);
    WiFi.begin(settings.ssid, settings.wifi_password);
    int max_delay = 20;
    while (WiFi.status() != WL_CONNECTED && max_delay-- > 0) {
      // Update screen
      Serial.print('.');
      delay(1000);
    }
    Serial.println();
  }

  String ipAddress;
  if (WiFi.status() != WL_CONNECTED) {
    Serial.printf("Failed to connect to WiFi '%s', starting AP mode\n", settings.ssid);
    WiFi.mode(WIFI_AP);
    char hostname[50];
    snprintf(hostname, sizeof(hostname), "%s_%d", OTA_HOSTNAME, random(9999));
    WiFi.softAP(hostname);
    ipAddress = WiFi.softAPIP().toString();
  } else {
    ipAddress = WiFi.localIP().toString();
  }
  Serial.printf("IP address: %s\n", ipAddress);
}
