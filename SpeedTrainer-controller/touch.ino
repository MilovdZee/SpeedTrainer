void setup_touch() {
  uint16_t calData[] = { 3831, 3721, 3848, 201, 263, 3699, 281, 203 };
  tft.setTouchCalibrate(calData);

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
  uint16_t touchX, touchY;
  bool touched = tft.getTouch(&touchX, &touchY);
  if (!touched) {
    data->state = LV_INDEV_STATE_REL;
  } else {
    data->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    data->point.x = SCREEN_WIDTH - touchX;
    data->point.y = SCREEN_HEIGHT - touchY;
  }
}

void calibrate_touch() {
  uint16_t calData[8];
  tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);
  Serial.print("uint16_t calData[] = { ");
  for (uint8_t i = 0; i < 8; i++) {
    Serial.print(calData[i]);
    if (i < 7) Serial.print(", ");
  }
  Serial.println(" };");
}
