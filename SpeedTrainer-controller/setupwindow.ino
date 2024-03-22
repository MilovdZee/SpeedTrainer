#define OK_BTN 1
#define CLOSE_BTN 2

lv_obj_t *setup_win;

lv_obj_t *ssid_input;
lv_obj_t *wifi_password_input;
lv_obj_t *brightness_input;

static void setup_window_event_handler(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_CLICKED) {
    lv_obj_t *obj = lv_event_get_target(event);
    int btnId = lv_obj_get_index(obj);
    Serial.printf("Button %d clicked: ", btnId);
    switch (btnId) {
      case OK_BTN:
        Serial.println("OK button");
        save_settings();

        // Wait a while before the restart
        delay(2000);
        ESP.restart();
        break;
      case CLOSE_BTN:
        Serial.println("Close button");
        lv_obj_del_async(setup_win);
        break;
      default:
        Serial.println("Unknown button");
    }
  }
}

void setup_window(boolean show_close_button) {
  setup_win = lv_win_create(get_screen_main(), 20);
  lv_win_add_title(setup_win, "Config");
  lv_obj_add_style(setup_win, &style_small, 0);
  lv_obj_t *btn = lv_win_add_btn(setup_win, LV_SYMBOL_OK, 30);
  lv_obj_add_event_cb(btn, setup_window_event_handler, LV_EVENT_CLICKED, NULL);

  if (show_close_button) {
    btn = lv_win_add_btn(setup_win, LV_SYMBOL_CLOSE, 30);
    lv_obj_add_event_cb(btn, setup_window_event_handler, LV_EVENT_CLICKED, NULL);
  }

  SettingsClass settings = get_settings();
  ssid_input = input_line(setup_win, "SSID: ", 0, settings.ssid);
  wifi_password_input = input_line(setup_win, "Password: ", 40, settings.wifi_password);
  brightness_input = input_slider(setup_win, "Brightness: ", 210, settings.brightness);

  char buffer[BUFFER_SIZE];
  snprintf(buffer, BUFFER_SIZE, "Firmware version: %d, MilovdZee", CURRENT_FIRMWARE_VERSION);
  label(setup_win, buffer, 0, 240);
}

void save_settings() {
  const char *ssid = lv_textarea_get_text(ssid_input);
  const char *wifi_password = lv_textarea_get_text(wifi_password_input);
  int brightness = lv_slider_get_value(brightness_input);

  Serial.println("Settings:");
  Serial.printf("  - ssid                 : '%s'\n", ssid);
  Serial.printf("  - wifi_password        : '%s'\n", wifi_password);
  Serial.printf("  - brightness           : '%d'\n", brightness);

  SettingsClass settings = SettingsClass(ssid, wifi_password, brightness);
  write_settings(settings);
}
