#define SETTINGS_NAME "SPDTRNRCTRLR"

Preferences preferences;
SettingsClass settings;

void read_settings() {
  // For the ESP the flash has to be read to a buffer
  preferences.begin(SETTINGS_NAME, false);

  int settingsSize = preferences.getBytesLength("settings");
  Serial.printf("'settings' data structure has size: %d\n", settingsSize);

  if (!preferences.getBytes("settings", &settings, sizeof(settings))) {
    Serial.println("ERROR: Failed to read preferences from EEPROM!");
    return;
  }

  Serial.printf("settings.eeprom_check: '%s'\n", settings.eeprom_check);
  if (check_validity(settings)) {
    Serial.println("EEPROM data found");
  } else {
    Serial.println("EEPROM data NOT found");
    settings = SettingsClass();
  }
}

void write_settings(SettingsClass newSettings) {
  preferences.begin(SETTINGS_NAME, false);
  if (!preferences.putBytes("settings", &newSettings, sizeof(newSettings))) {
    Serial.println("ERROR: Failed to write preferences to EEPROM!");
    return;
  }
  settings = newSettings;
}

boolean check_validity(SettingsClass settings) {
  return strncmp(settings.eeprom_check, EEPROM_CHECK_STRING, sizeof(EEPROM_CHECK_STRING)) == 0;
}

SettingsClass get_settings() {
  return settings;
}
