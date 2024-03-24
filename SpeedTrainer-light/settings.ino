Preferences preferences;

Settings settings;

void read_settings() {
  // For the ESP the flash has to be read to a buffer
  preferences.begin(HOSTNAME, false);

  int settings_size = preferences.getBytesLength("settings");
  Serial.printf("'settings' data structure has size: %d\n", settings_size);

  if (!preferences.getBytes("settings", &settings, sizeof(settings))) {
    Serial.println("ERROR: Failed to read preferences from EEPROM!");
    return;
  }

  Serial.printf("settings.eeprom_check: '%s'\n", settings.eeprom_check);
  if (check_validity(settings)) {
    Serial.println("EEPROM data found");
  } else {
    Serial.println("EEPROM data NOT found");
    settings = Settings();
  }
  show_settings(settings);
}

void write_settings(Settings newSettings) {
  preferences.begin(HOSTNAME, false);
  int written = preferences.putBytes("settings", &newSettings, sizeof(newSettings));
  if (!written) {
    Serial.println("ERROR: Failed to write preferences to EEPROM!");
    return;
  } else {
    Serial.printf("Written %d bytes to EEPROM\n", written);
  }
  show_settings(newSettings);
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
