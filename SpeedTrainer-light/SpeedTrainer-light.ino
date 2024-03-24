#include "BLEDevice.h"
#include <ArduinoOTA.h>
#include <Preferences.h>
#include <WebServer.h>
#include "config.h"

void setup() {
  Serial.begin(115200);
  Serial.setTxBufferSize(SERIAL_BUFFER_SIZE);
  Serial.println("Setup started");

  digitalWrite(LED, HIGH);
  pinMode(LED, OUTPUT);

  read_settings();

  setup_wifi();
  setup_ota();
  setup_web();
  setup_ble();

  Serial.println("Setup finished");
}

uint8_t value = 0;
void loop() {
  digitalWrite(LED, LOW);
  notify(value++);
  delay_for_millis(50);
  digitalWrite(LED, HIGH);

  delay_for_millis(2000);
}

void delay_for_millis(int delay) {
  unsigned long start = millis();
  while (millis() - start < delay) {
    ArduinoOTA.handle();
    handle_web_client();
  }
}
