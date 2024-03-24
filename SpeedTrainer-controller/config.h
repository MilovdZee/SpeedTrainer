#ifndef CONFIG_H
#define CONFIG_H

#define SERIAL_BUFFER_SIZE 1000

#define EEPROM_CHECK_STRING "EEPROM001"
#define CURRENT_FIRMWARE_VERSION 2

/* Change to your screen resolution */
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define HEADER_HEIGHT 30

#define RGB_RED_PIN 4
#define RGB_GREEN_PIN 16
#define RGB_BLUE_PIN 17

#define HOSTNAME "SPDTRNRCTRLR"

#define OTA_PASSWORD "SpeedTrainer"
#define OTA_HOSTNAME "SpeedTrainerController"

#define BUFFER_SIZE 500
#define ERROR_VALUE -1

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// NTP server
const char *ntpServer = "pool.ntp.org";
char *timezone = "CET-1CEST,M3.5.0,M10.5.0/3";
char *timezone_text = "Amsterdam";

class Settings {
public:
  char eeprom_check[10];
  char ssid[60];
  char wifi_password[60];
  int brightness;

  Settings() {
    strncpy(this->eeprom_check, EEPROM_CHECK_STRING, sizeof(this->eeprom_check));
    ssid[0] = 0;
    wifi_password[0] = 0;
    brightness = 50;
  };
};

#endif