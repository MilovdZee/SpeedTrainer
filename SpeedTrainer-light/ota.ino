static void ota_start() {
  Serial.println("OTA update start");
}

static void ota_end() {
  Serial.println("OTA update end");
}

void ota_on_progress(int progress, int total) {
  static int last_shown = 0;
  int percentage = progress * 100 / total;
  if (last_shown != percentage) {
    if (percentage % 10 == 0) {
      Serial.printf("OTA update progress: %u\r\n", percentage);
    }

    last_shown = percentage;
  }
}

void setup_ota() {
  if (WiFi.status() != WL_CONNECTED) return;

  // Setup OTA updates
  ArduinoOTA.setPort(3232);  // default 3232
  char hostname[50];
  snprintf(hostname, sizeof(hostname), "%s_%d", OTA_HOSTNAME, random(9999));
  ArduinoOTA.setHostname(hostname);
  ArduinoOTA.setPassword(OTA_PASSWORD);

  ArduinoOTA.onStart(ota_start);
  ArduinoOTA.onEnd(ota_end);
  ArduinoOTA.onProgress(ota_on_progress);
  ArduinoOTA.onError([](ota_error_t error) {
    const char* errorMessage = "Unknown";
    if (error == OTA_AUTH_ERROR) errorMessage = "Auth Failed";
    else if (error == OTA_BEGIN_ERROR) errorMessage = "Begin Failed";
    else if (error == OTA_CONNECT_ERROR) errorMessage = "Connect Failed";
    else if (error == OTA_RECEIVE_ERROR) errorMessage = "Receive Failed";
    else if (error == OTA_END_ERROR) errorMessage = "End Failed";
    Serial.printf("Error[%u]: %s\n", error, errorMessage);
  });
  ArduinoOTA.begin();
}
