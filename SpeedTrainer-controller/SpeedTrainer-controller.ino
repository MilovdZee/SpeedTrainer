#include <LovyanGFX.hpp>
#include <lvgl.h>
#include <Preferences.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoOTA.h>
#include <Update.h>
#include "esp_sntp.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include "lgfx_ESP32_2432S028.h"
#include "config.h"

static lv_style_t style_status_label;
static lv_style_t style_default;
static lv_style_t style_small;

void setup() {
  Serial.begin(115200);
  Serial.setTxBufferSize(SERIAL_BUFFER_SIZE);
  Serial.println("Setup started");

  // Switch off the RGB LED
  digitalWrite(RGB_RED_PIN, HIGH);
  pinMode(RGB_RED_PIN, OUTPUT);
  digitalWrite(RGB_GREEN_PIN, HIGH);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  digitalWrite(RGB_BLUE_PIN, HIGH);
  pinMode(RGB_BLUE_PIN, OUTPUT);

  bool validSettings = read_settings();
  setup_screen();
  setup_touch();
  //setup_ble();
  if (validSettings) {
    setup_wifi();
    setup_ota();
    setup_ntp();

    main_window();
  } else {
    show_status_label("Not configured", true);
    setup_window(false);
  }

  Serial.println("Setup finished");

  if (WiFi.status() == WL_CONNECTED) {
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "Checking for firmware updates... (installed v%d)\n", CURRENT_FIRMWARE_VERSION);
    show_status_label(buffer, false);
    lv_timer_handler();
    Serial.println(buffer);
    int newest_version = get_update_version();
    hide_status_label();
    if (newest_version > CURRENT_FIRMWARE_VERSION) {
      show_status_label("Updating firmware...", false);
      lv_timer_handler();
      update_firmware(newest_version);
    }
  }
}

uint8_t value = 0;
void loop() {
  digitalWrite(RGB_BLUE_PIN, LOW);
  notify(value++);
  delay_for_millis(50);
  digitalWrite(RGB_BLUE_PIN, HIGH);

  delay_for_millis(3000);
}

void delay_for_millis(int delay) {
  unsigned long start = millis();
  while (millis() - start < delay) {
    ArduinoOTA.handle();
    lv_timer_handler_run_in_period(5);
  }
}
