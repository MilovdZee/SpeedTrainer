#define OK_BTN 1
#define CLOSE_BTN 2

#define RED1 6
#define RED2 7
#define GREEN 8
#define BLUE 9
#define YELLOW 10
#define WHITE 11

#define BUTTON_WIDTH 40
#define BUTTON_HIGHT 30
#define BUTTON_SPACING 10

lv_obj_t *setup_win;

lv_obj_t *ssid_input;
lv_obj_t *wifi_password_input;
lv_obj_t *brightness_input;

static void setup_window_header_event_handler(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_CLICKED) {
    lv_obj_t *obj = lv_event_get_target(event);
    int btnId = lv_obj_get_index(obj);
    Serial.printf("Button %d clicked: ", btnId);
    switch (btnId) {
      case OK_BTN:
        Serial.println("OK");
        save_settings();

        // Wait a while before the restart
        delay(2000);
        ESP.restart();
        break;
      case CLOSE_BTN:
        Serial.println("Close");
        lv_obj_del_async(setup_win);
        break;
      default:
        Serial.println("Unknown");
    }
  }
}

static void setup_window_content_event_handler(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_CLICKED) {
    lv_obj_t *obj = lv_event_get_target(event);
    int btnId = lv_obj_get_index(obj);
    Serial.printf("Button %d clicked: ", btnId);
    switch (btnId) {
      case RED1:
        Serial.println("RED1");
        break;
      case RED2:
        Serial.println("RED2");
        break;
      case GREEN:
        Serial.println("GREEN");
        break;
      case BLUE:
        Serial.println("BLUE");
        break;
      case YELLOW:
        Serial.println("YELLOW");
        break;
      case WHITE:
        Serial.println("WHITE");
        break;
      default:
        Serial.println("Unknown");
    }
  }
}

void setup_window(boolean show_close_button) {
  setup_win = lv_win_create(get_screen_main(), 20);
  lv_win_add_title(setup_win, "Config");
  lv_obj_add_style(setup_win, &style_small, 0);
  lv_obj_t *btn = lv_win_add_btn(setup_win, LV_SYMBOL_OK, 30);
  lv_obj_add_event_cb(btn, setup_window_header_event_handler, LV_EVENT_CLICKED, NULL);

  if (show_close_button) {
    btn = lv_win_add_btn(setup_win, LV_SYMBOL_CLOSE, 30);
    lv_obj_add_event_cb(btn, setup_window_header_event_handler, LV_EVENT_CLICKED, NULL);
  }

  Settings settings = get_settings();

  ssid_input = input_line(setup_win, "SSID: ", 0, settings.ssid);
  wifi_password_input = input_line(setup_win, "Password: ", 40, settings.wifi_password);
  brightness_input = input_slider(setup_win, "Brightness: ", 90, settings.brightness);

  button(setup_win, "R1", BUTTON_WIDTH, BUTTON_HIGHT, -2.5 * (BUTTON_WIDTH + BUTTON_SPACING), 120, setup_window_content_event_handler);
  button(setup_win, "R2", BUTTON_WIDTH, BUTTON_HIGHT, -1.5 * (BUTTON_WIDTH + BUTTON_SPACING), 120, setup_window_content_event_handler);
  button(setup_win, "G", BUTTON_WIDTH, BUTTON_HIGHT, -0.5 * (BUTTON_WIDTH + BUTTON_SPACING), 120, setup_window_content_event_handler);
  button(setup_win, "B", BUTTON_WIDTH, BUTTON_HIGHT, 0.5 * (BUTTON_WIDTH + BUTTON_SPACING), 120, setup_window_content_event_handler);
  button(setup_win, "Y", BUTTON_WIDTH, BUTTON_HIGHT, 1.5 * (BUTTON_WIDTH + BUTTON_SPACING), 120, setup_window_content_event_handler);
  button(setup_win, "W", BUTTON_WIDTH, BUTTON_HIGHT, 2.5 * (BUTTON_WIDTH + BUTTON_SPACING), 120, setup_window_content_event_handler);

  char buffer[BUFFER_SIZE];
  snprintf(buffer, BUFFER_SIZE, "Firmware version: %d, MilovdZee", CURRENT_FIRMWARE_VERSION);
  label(setup_win, buffer, 0, 170);
}

void save_settings() {
  Settings settings = get_settings();
  snprintf(settings.ssid, sizeof(settings.ssid), lv_textarea_get_text(ssid_input));
  snprintf(settings.wifi_password, sizeof(settings.wifi_password), lv_textarea_get_text(wifi_password_input));
  settings.brightness = lv_slider_get_value(brightness_input);

  Serial.println("Settings:");
  Serial.printf("  - ssid                 : '%s'\n", settings.ssid);
  Serial.printf("  - wifi_password        : '%s'\n", settings.wifi_password);
  Serial.printf("  - brightness           : '%d'\n", settings.brightness);

  write_settings(settings);
}
