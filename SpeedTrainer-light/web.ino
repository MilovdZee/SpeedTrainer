WebServer server(80);

void setup_web() {
  server.on("/", handle_root);
  server.on("/wifi", handle_wifi);
  server.begin();
}

void handle_web_client() {
  server.handleClient();
}

void handle_root() {
  bool do_reset = false;
  Settings settings = get_settings();

  if (server.method() == HTTP_POST) {
    unsigned short brightness = server.arg("brightness").toInt();
    settings.brightness = brightness;
    write_settings(settings);

    do_reset = true;
  }

  String rootForm = String(css) + "<html>\
    <body>\
      <div class=\"container\">\
        <h1>Settings</h1>\
        <form method=\"POST\" action=\"/\">\
          <div>\
            <div>Brightness:</div>\
            <div><input type=\"range\" name=\"brightness\" min=\"0\" max=\"20\" value=\""
                    + String(settings.brightness) + "\"></div>\
          </div>\
          </br>\
          <input type=\"submit\" value=\"Submit\">\
        </form>\
        <div class=\"wifi_link\"><a href=\"/wifi\">wifi</a></div>\
      </div>\
    </body>\
  </html>";

  server.send(200, "text/html", rootForm);

  if (do_reset) {
    delay_for_millis(500);
    ESP.restart();
  }
}

void handle_wifi() {
  bool do_reset = false;
  Settings settings = get_settings();

  if (server.method() == HTTP_POST) {
    snprintf(settings.ssid, sizeof(settings.ssid), server.arg("ssid").c_str());
    snprintf(settings.wifi_password, sizeof(settings.wifi_password), server.arg("password").c_str());
    write_settings(settings);

    do_reset = true;
  }

  // Read back to check if the values are stored correctly
  String wifiForm = String(css) + "<html>\
    <body>\
      <div class=\"container\">\
      <h1>WiFi Settings</h1>\
        <form method=\"POST\" action=\"/wifi\">\
          SSID:</br>\
          <input type=\"text\" name=\"ssid\" value=\""
                    + String(settings.ssid) + "\"></br></br>\
          Password:</br>\
          <input type=\"text\" name=\"password\" value=\""
                    + String(settings.wifi_password) + "\">\
          <input type=\"submit\" value=\"Submit\">\
        </form>\
      </div>\
    </body>\
  </html>";

  server.send(200, "text/html", wifiForm);

  if (do_reset) {
    delay_for_millis(500);
    ESP.restart();
  }
}
