/*
   Based on 31337Ghost's reference code from https://github.com/nkolban/esp32-snippets/issues/385#issuecomment-362535434
   which is based on pcbreflux's Arduino ESP32 port of Neil Kolban's example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
*/

/*
   Create a BLE server that will send periodic iBeacon frames.
   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create advertising data
   3. Start advertising.
   4. wait
   5. Stop advertising.
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <BLEBeacon.h>
//#include <mbedtls/aes.h>

// Generated using: https://www.uuidgenerator.net/version4
#define DEVICE_NAME         "Blue Pulse Dev"
#define BEACON_UUID_REV     "e485ef7e-a0c8-4e8b-9c9b-7d8db9007a32"

#define MAJOR 5
#define MINOR 80

BLEServer *pServer;

void init_beacon() {
  BLEAdvertising *pAdvertising;
  pAdvertising = pServer->getAdvertising();
  pAdvertising->stop();
  // iBeacon
  // More info: https://www.bluetooth.com/wp-content/uploads/Files/Specification/HTML/Assigned_Numbers/out/en/Assigned_Numbers.pdf?v=1715088212287
  BLEBeacon myBeacon;
  myBeacon.setManufacturerId(0x4c00);
  myBeacon.setMajor(MAJOR);
  myBeacon.setMinor(MINOR);
  myBeacon.setSignalPower(0xCD); //-51 Dbw for 1m range from beacon
  myBeacon.setProximityUUID(BLEUUID(BEACON_UUID_REV));

  BLEAdvertisementData advertisementData;
  advertisementData.setFlags(0x1A);
  advertisementData.setManufacturerData(myBeacon.getData());
  pAdvertising->setAdvertisementData(advertisementData);

  pAdvertising->setAdvertisementType(ADV_TYPE_NONCONN_IND); // Non-connectable undirected advertising
  pAdvertising->start();
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Initializing...");
  Serial.flush();

  BLEDevice::init(DEVICE_NAME);
  BLEDevice::setPower(ESP_PWR_LVL_P20); // Set transmit power to +20 dBm

  pServer = BLEDevice::createServer();

  init_beacon();

  Serial.println("iBeacon + service defined and advertising!");
}

void loop() {
  // No need for loop content in beacon-only mode
}