Preferences preferences;
Settings settings;

bool read_settings() {
  // For the ESP the flash has to be read to a buffer
  preferences.begin(HOSTNAME, false);

  int settingsSize = preferences.getBytesLength("settings");
  Serial.printf("'settings' data structure has size: %d\n", settingsSize);

  if (!preferences.getBytes("settings", &settings, sizeof(settings))) {
    Serial.println("ERROR: Failed to read preferences from EEPROM!");
    return false;
  }

  Serial.printf("settings.eeprom_check: '%s'\n", settings.eeprom_check);
  bool valid = check_validity(settings);
  if (valid) {
    Serial.println("EEPROM data found");
  } else {
    Serial.println("EEPROM data NOT found");
    settings = Settings();
  }
  show_settings(settings);
  return valid;
}

void write_settings(Settings newSettings) {
  preferences.begin(HOSTNAME, false);
  if (!preferences.putBytes("settings", &newSettings, sizeof(newSettings))) {
    Serial.println("ERROR: Failed to write preferences to EEPROM!");
    return;
  }
  settings = newSettings;
}

boolean check_validity(Settings settings) {
  return strncmp(settings.eeprom_check, EEPROM_CHECK_STRING, sizeof(EEPROM_CHECK_STRING)) == 0;
}

Settings get_settings() {
  return settings;
}

void show_settings() {
  show_settings(settings);
}

void show_settings(Settings settings) {
  Serial.printf("\nSettings:\n");
  Serial.printf("  - ssid                 : '%s'\n", settings.ssid);
  Serial.printf("  - wifi_password        : '%s'\n", settings.wifi_password);
  Serial.printf("  - brightness           : '%d'\n", settings.brightness);
}