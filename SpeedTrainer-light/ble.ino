static BLEAdvertisedDevice* controllerDevice = nullptr;
static BLERemoteCharacteristic* pRemoteCharacteristic = nullptr;

static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  digitalWrite(LED, LOW);
  Serial.printf("Received value: %hhu\n", *pData);
  delay(50);
  digitalWrite(LED, HIGH);
}

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
    //Serial.printf("onConnect: '%s'\n", pclient->getPeerAddress().toString().c_str());
  }

  void onDisconnect(BLEClient* pclient) {
    Serial.printf("onDisconnect: '%s'\n", pclient->getPeerAddress().toString().c_str());
    pRemoteCharacteristic = nullptr;
  }
};

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(BLEUUID(SERVICE_UUID))) {
      Serial.println("Found searched BLE server");

      BLEDevice::getScan()->stop();
      controllerDevice = new BLEAdvertisedDevice(advertisedDevice);
    }
  }
};

void setup_ble() {
  delay_for_millis(500);
  BLEDevice::init(HOSTNAME);

  // Setup a scanner
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
}

void notify(uint8_t value) {
  if (pRemoteCharacteristic == nullptr) {
    connectToServer();
  }

  if (pRemoteCharacteristic != nullptr) {
    pRemoteCharacteristic->writeValue(&value, 1);
  }
}

bool connectToServer() {
  if(controllerDevice == nullptr) {
    Serial.println("SpeedTrainer controller not seen yet. Starting a scan...");
    BLEDevice::getScan()->start(5, false);
    if(controllerDevice == nullptr) {
      return false;
    }
  }

  Serial.printf("Forming a connection to '%s'\n", controllerDevice->getAddress().toString().c_str());

  BLEClient* pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());
  Serial.println(" - Created client");

  // Connect to the remove BLE Server.
  Serial.println(" - Connecting to server...");
  pClient->connect(controllerDevice);
  bool isConnected = pClient->isConnected();
  Serial.printf(" - Connecting to server: %s\n", isConnected ? "SUCCESS" : "FAILED");
  if(!isConnected) {
    return false;
  }
  pClient->setMTU(517);

  // Request a list of services
  std::map<std::string, BLERemoteService*>* services = pClient->getServices();
  if (services != nullptr) {
    Serial.printf("  - Found %d services\n", services->size());
    for (const auto& [key, _] : *services) {
      Serial.printf("    - '%s'\n", key.c_str());
    }
  }

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
  if (pRemoteService == nullptr) {
    Serial.printf("Failed to find our service UUID: '%s'\n", SERVICE_UUID);
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our service");

  // Obtain a reference to the characteristic in the service of the remote BLE server.
  pRemoteCharacteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID);
  if (pRemoteCharacteristic == nullptr) {
    Serial.printf("Failed to find our characteristic UUID: '%s'\n", CHARACTERISTIC_UUID);
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our characteristic");

  if (pRemoteCharacteristic->canNotify()) {
    pRemoteCharacteristic->registerForNotify(notifyCallback);
  }

  return true;
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
