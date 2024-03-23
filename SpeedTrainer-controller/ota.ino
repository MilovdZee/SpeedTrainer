static lv_obj_t* ota_progress_meter;
lv_meter_indicator_t* ota_progress_indicator;

static void ota_start() {
  hide_status_label();

  digitalWrite(RGB_RED_PIN, LOW);

  lv_obj_t* main = get_screen_main();
  lv_obj_clean(main);
  lv_obj_add_style(main, &style_small, 0);

  ota_progress_meter = lv_meter_create(main);
  lv_obj_center(ota_progress_meter);
  lv_obj_set_size(ota_progress_meter, 200, 200);

  lv_obj_remove_style(ota_progress_meter, NULL, LV_PART_INDICATOR);

  lv_meter_scale_t* scale = lv_meter_add_scale(ota_progress_meter);
  lv_meter_set_scale_ticks(ota_progress_meter, scale, 11, 2, 10, lv_palette_main(LV_PALETTE_GREY));
  lv_meter_set_scale_major_ticks(ota_progress_meter, scale, 1, 2, 20, lv_color_hex3(0xeee), 10);
  lv_meter_set_scale_range(ota_progress_meter, scale, 0, 100, 270, 90);

  ota_progress_indicator = lv_meter_add_arc(ota_progress_meter, scale, 10, lv_palette_main(LV_PALETTE_RED), 0);
  lv_meter_set_indicator_start_value(ota_progress_meter, ota_progress_indicator, 0);
  lv_meter_set_indicator_end_value(ota_progress_meter, ota_progress_indicator, 0);

  lv_timer_handler();
}

void ota_on_progress(int progress, int total) {
  static int last_shown = 0;
  int percentage = progress * 100 / total;
  if (last_shown != percentage) {
    if (percentage % 10 == 0) {
      Serial.printf("OTA update progress: %u\r\n", percentage);
    }
    lv_meter_set_indicator_end_value(ota_progress_meter, ota_progress_indicator, percentage);
    lv_timer_handler();
    last_shown = percentage;
  }
}

void setup_ota() {
  if (WiFi.status() != WL_CONNECTED) return;

  // Setup OTA updates
  ArduinoOTA.setPort(3232);  // default 3232
  ArduinoOTA.setHostname(OTA_HOSTNAME);
  ArduinoOTA.setPassword(OTA_PASSWORD);

  ArduinoOTA.onStart([]() {
    Serial.println("OTA update start");
    ota_start();
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("OTA update end");
  });
  ArduinoOTA.onProgress(ota_on_progress);
  ArduinoOTA.onError([](ota_error_t error) {
    char* errorMessage = "Unknown";
    if (error == OTA_AUTH_ERROR) errorMessage = "Auth Failed";
    else if (error == OTA_BEGIN_ERROR) errorMessage = "Begin Failed";
    else if (error == OTA_CONNECT_ERROR) errorMessage = "Connect Failed";
    else if (error == OTA_RECEIVE_ERROR) errorMessage = "Receive Failed";
    else if (error == OTA_END_ERROR) errorMessage = "End Failed";
    Serial.printf("Error[%u]: %s\n", error, errorMessage);
  });
  ArduinoOTA.begin();
}
