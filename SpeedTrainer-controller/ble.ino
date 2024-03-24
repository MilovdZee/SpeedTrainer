BLEServer* pServer = nullptr;
BLECharacteristic* pCharacteristic = nullptr;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t* param) {
    Serial.printf("Client '%s' connected\n", mac2string(param->connect.remote_bda).c_str());
    BLEDevice::startAdvertising();
  };

  void onDisconnect(BLEServer* pServer, esp_ble_gatts_cb_param_t* param) {
    Serial.printf("Client '%s' disconnected\n", mac2string(param->connect.remote_bda).c_str());
    Serial.println("Disconnected");
  }
};

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    digitalWrite(RGB_GREEN_PIN, LOW);
    const uint8_t* value = pCharacteristic->getData();
    Serial.printf("Received value: '%hhu'\n", *value);
    delay_for_millis(50);
    digitalWrite(RGB_GREEN_PIN, HIGH);
  }
};

void setup_ble() {
  delay_for_millis(500);
  BLEDevice::init(HOSTNAME);

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService* pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE);
  pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());

  // Start the service
  pService->start();

  // Setup advertising
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  BLEDevice::startAdvertising();
}

void notify(uint8_t value) {
  if (pCharacteristic != nullptr) {
    pCharacteristic->setValue(&value, 1);
    pCharacteristic->notify();
  }
}

String mac2string(byte ar[]) {
  String s;
  for (byte i = 0; i < 6; ++i) {
    char buf[3];
    sprintf(buf, "%02X", ar[i]);
    s += buf;
    if (i < 5) s += ':';
  }
  return s;
}
