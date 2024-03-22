#define UPDATE_HOST "raw.githubusercontent.com"
#define UPDATE_PORT 443
#define VERSION_FILE_LOCATION "/MilovdZee/SpeedTrainer/main/firmware/controller_version.txt"
#define FIRMWARE_FILE_LOCATION "/MilovdZee/SpeedTrainer/main/firmware/SpeedTrainerController.ino-%d.bin"

#define CONNECTION_TIMEOUT 5000

WiFiClientSecure client;

const char * root_ca = \
  "-----BEGIN CERTIFICATE-----\n" \
  "MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n" \
  "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
  "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n" \
  "QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n" \
  "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n" \
  "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n" \
  "9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n" \
  "CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n" \
  "nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n" \
  "43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n" \
  "T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n" \
  "gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n" \
  "BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n" \
  "TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n" \
  "DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n" \
  "hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n" \
  "06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n" \
  "PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n" \
  "YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n" \
  "CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n" \
  "-----END CERTIFICATE-----\n";

String get_header_value(String header) {
  char *space = strchr(header.c_str(), ' ');
  int index_of_space = space ? space - header.c_str() + 1 : -1;
  if (index_of_space != -1) {
    return header.substring(index_of_space);
  }
  return "";
}

boolean check_for_data() {
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println("Client Timeout!");
      return false;
    }
  }
  return true;
}

long read_content_length_from_headers() {
  long content_length = ERROR_VALUE;
  while (client.available()) {
    String line = client.readStringUntil('\n');
    line.trim();

    // end of headers
    if (line.length() == 0) break;

    // check if the HTTP response is 200
    if (line.startsWith("HTTP/1.1")) {
      if (line.indexOf("200") < 0) {
        Serial.printf("Got a non 200 status code from server: '%s'\n", line.c_str());
        return ERROR_VALUE;
      }
    }

    if (line.startsWith("Content-Length: ")) {
      content_length = atol(get_header_value(line).c_str());
      Serial.printf("  - Content length: %d bytes\n", content_length);
    }

    if (line.startsWith("Content-Type: ")) {
      const char *content_type = get_header_value(line).c_str();
      Serial.printf("  - Content type: '%s'\n", content_type);
    }
  }

  return content_length;
}

boolean connect_to_host() {
  if (WiFi.status() != WL_CONNECTED) return false;

  // set certificate handling
  //client.setInsecure();
  client.setCACert(root_ca);

  // connect to the host
  if (!client.connect(UPDATE_HOST, UPDATE_PORT)) {
    Serial.printf("Connection failed: '%s:%d'\n", UPDATE_HOST, UPDATE_PORT);
    return false;
  }
  Serial.printf("Connected to: '%s:%d'\n", UPDATE_HOST, UPDATE_PORT);

  return true;
}

int get_update_version() {
  if (!connect_to_host()) return ERROR_VALUE;

  // request the version file
  Serial.printf("Requesting '%s'\n", VERSION_FILE_LOCATION);
  String get_request = String("GET ") + VERSION_FILE_LOCATION + " HTTP/1.0\r\n" + "Host: " + UPDATE_HOST + "\r\n" + "User-Agent: ESP32\r\n" + "Cache-Control: no-cache\r\n" + "\r\n";
  client.print(get_request);

  int newest_version = ERROR_VALUE;
  if (check_for_data()) {
    long content_length = read_content_length_from_headers();

    // read the first line of the version file
    if (content_length != ERROR_VALUE && client.available()) {
      String line = client.readStringUntil('\n');
      line.trim();
      newest_version = atoi(line.c_str());
    }
  }
  Serial.printf("Newest firmware version: %d\n", newest_version);

  // stop the connection
  client.stop();

  // validate version
  if (newest_version < 1 || newest_version > 100) return ERROR_VALUE;
  return newest_version;
}

void update_firmware(int version) {
  if (!connect_to_host()) return;

  // request the version file
  char buffer[BUFFER_SIZE];
  snprintf(buffer, BUFFER_SIZE, FIRMWARE_FILE_LOCATION, version);
  Serial.printf("Requesting '%s'\n", buffer);
  String get_request = String("GET ") + buffer + " HTTP/1.0\r\n" + "Host: " + UPDATE_HOST + "\r\n" + "User-Agent: ESP32\r\n" + "\r\n";
  client.print(get_request);

  if (check_for_data()) {
    long content_length = read_content_length_from_headers();
    if (content_length != ERROR_VALUE) {
      bool can_begin = Update.begin(content_length);
      if (can_begin) {
        Serial.printf("Begin OTA of %ld bytes...\n", content_length);
        hide_status_label();
        ota_start();
        Update.onProgress(ota_on_progress);

        size_t written = Update.writeStream(client);

        if (written == content_length) {
          Serial.println("Firmware written successfully");
        } else {
          Serial.println("Firmware not completely written!");
        }

        if (Update.end()) {
          if (Update.isFinished()) {
            Serial.println("Finished OTA");
            ESP.restart();
          } else {
            Serial.println("Unfinished OTA!");
          }
        } else {
          Serial.printf("Update failed: %s\n", Update.getError());
        }
      } else {
        Serial.println("OTA has not enough space!");
        client.flush();
      }
    } else {
      Serial.println("OTA has no data available!");
      client.flush();
    }
  }

  // stop the connection
  client.stop();
}
