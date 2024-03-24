void setup_wifi() {
  Settings settings = get_settings();
  if (settings.ssid[0] == 0) return;

  int bufSize = 128;
  char status[bufSize];
  snprintf(status, bufSize, "Connecting to:\n#01117a %s#\n.", settings.ssid);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(settings.ssid, settings.wifi_password);
  int max_delay = 20;
  while (WiFi.status() != WL_CONNECTED && max_delay-- > 0) {
    // Update screen
    show_status_label(status, false);
    delay_for_millis(1000);
    strncat(status, ".", sizeof(status));
    hide_status_label();
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect to WiFi");
    snprintf(status, bufSize, "Failed to connect to:\n#01117a %s#", settings.ssid);
    show_status_label(status, true);
  } else {
    String ipAddress = WiFi.localIP().toString();
    Serial.printf("Connected to WiFi: %s\n", ipAddress);
    snprintf(status, bufSize, "Connected to\n#01117a %s#\n#01520a %s#", settings.ssid, ipAddress);
    show_status_label(status, false);
    delay_for_millis(2000);
    hide_status_label();
  }
}