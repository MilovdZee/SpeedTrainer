#ifndef CONFIG_H
#define CONFIG_H

#define SERIAL_BUFFER_SIZE 2000

#define EEPROM_CHECK_STRING "EEPROM001"
#define CURRENT_FIRMWARE_VERSION 1

#define HOSTNAME "SPDTRNRLGHT"
#define CONTROLLER_HOSTNAME "SPDTRNRCTRLR"

#define OTA_PASSWORD "SpeedTrainer"
#define OTA_HOSTNAME "SpeedTrainerLight"

#define BUFFER_SIZE 300
#define ERROR_VALUE -1

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define LED 8

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
    brightness = 10;
  };
};

const char css[] PROGMEM = R"=====(
<style>
  body {
    background: #fad7a0;
    color: #154360;
    padding: 20px;
    font-size: 3em;
    text-align: center;
  }
  div.container {
    display: inline-block;
    width: 90%;
    height: 90%;
    background: #f8c471;
    box-shadow: 15px 20px 20px #88888888;
    border-radius: 20px;
    padding: 2%;
    text-align: left;
  }
  h1 {
    margin-top: 0;
  }
  form div {
    display: flex;
    align-items:flex-end;
    width: 100%;
    margin-bottom: 5px;
  }
  form div div {
    width: 9em;
  }
  form div div + div {
    flex-grow: 1;
  }
  input {
    width: 100%;
    border: 0;
    border-bottom: 2px solid grey;
    background: none;
    color: #154360;
    font-size: 1.2em;
  }
  input[type="range"] {
    width: 100%;
  }
  input[type="submit"] {
    background: #154360;
    color: #fad7a0;
    border: 0;
    border-radius: 5px;
    width: 40%;
    height: 10%;
    cursor: pointer;
    font-size: 1em;
    position: absolute;
    left: 30%;
    bottom: 10%;
  }
  .wifi_link {
    position: absolute;
    right: 2%;
    bottom: 2%;
    font-size: .6em;
  }
</style>
)=====";

#endif