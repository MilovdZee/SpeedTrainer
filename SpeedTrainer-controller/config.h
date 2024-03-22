#ifndef CONFIG_H
#define CONFIG_H

#define EEPROM_CHECK_STRING "EEPROM001"
#define CURRENT_FIRMWARE_VERSION 2

/* Change to your screen resolution */
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define HEADER_HEIGHT 30

#define RGB_RED_PIN 4
#define RGB_GREEN_PIN 16
#define RGB_BLUE_PIN 17

#define OTA_PASSWORD "SpeedTrainer"
#define OTA_HOSTNAME "SpeedTrainerController"

#define BUFFER_SIZE 300

#define ERROR_VALUE -1

// NTP server
const char *ntpServer = "pool.ntp.org";
char *timezone = "CET-1CEST,M3.5.0,M10.5.0/3";
char *timezone_text = "Amsterdam";

class SettingsClass {
public:
  char eeprom_check[10];
  char ssid[60];
  char wifi_password[60];
  int brightness;

  SettingsClass() {
    eeprom_check[0] = 0;
    ssid[0] = 0;
    wifi_password[0] = 0;
    brightness = 50;
  };

  SettingsClass(const char *ssid, const char *wifi_password, int brightness) {
    strncpy(this->eeprom_check, EEPROM_CHECK_STRING, sizeof(this->eeprom_check));
    strncpy(this->ssid, ssid, sizeof(this->ssid));
    strncpy(this->wifi_password, wifi_password, sizeof(this->wifi_password));
    this->brightness = brightness;
  };
};

#endif