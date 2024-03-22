void setup_ntp() {
  // Init and get the time
  sntp_set_sync_interval(12 * 60 * 60 * 1000UL);   // sync clock every 12 hours
  sntp_set_time_sync_notification_cb(cb_sync_time);  // set a Callback function for time synchronization notification
  configTime(0, 0, ntpServer);

  set_timezone(timezone);
}

void set_timezone(String timezone) {
  Serial.printf("Setting Timezone to %s\n", timezone.c_str());
  setenv("TZ", timezone.c_str(), 1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
}

// callback function to show when NTP was synchronized
void cb_sync_time(struct timeval *tv) {
  Serial.print("NTP time synched: ");
  print_local_time();
  Serial.println();
}

void print_local_time() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
  } else {
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S zone %Z %z");
  }
}
