// Based on https://github.com/adafruit/Adafruit_nRF52_Arduino
// /tree/master/libraries/Bluefruit52Lib/examples/Peripheral
// Copyright (c) Adafruit.com, all rights reserved.

// Licensed under the MIT license, see LICENSE or
// https://choosealicense.com/licenses/mit/

#include "Adafruit_TinyUSB.h"  // Fix https://github.com/adafruit/Adafruit_nRF52_Arduino/issues/653
#include <bluefruit.h>
#include "Arduino.h"

#define PUMP_MS_PER_ML (15)

BLEDis deviceInfoService;

BLEBas batteryService;

// plantWateringService   UUID: DDA50001-106D-4D32-949F-D07461C6C2FA
// threshold characteristic   UUID: DDA50002-106D-4D32-949F-D07461C6C2FA
// humidity Notify characteristic   UUID: DDA50003-106D-4D32-949F-D07461C6C2FA


// The arrays below are ordered "least significant byte first":
uint8_t const plantWateringServiceUuid[] =  { 0xFA, 0xC2, 0xC6, 0x61, 0x74, 0xD0, 0x9F, 0x94, 0x32, 0x4D, 0x6D, 0x10, 0x01, 0x00, 0xA5, 0xDD };
uint8_t const thresholdCharacteristicUuid[] =  { 0xFA, 0xC2, 0xC6, 0x61, 0x74, 0xD0, 0x9F, 0x94, 0x32, 0x4D, 0x6D, 0x10, 0x02, 0x00, 0xA5, 0xDD };
uint8_t const humidityNotifyCharacteristicUuid[] =  { 0xFA, 0xC2, 0xC6, 0x61, 0x74, 0xD0, 0x9F, 0x94, 0x32, 0x4D, 0x6D, 0x10, 0x03, 0x00, 0xA5, 0xDD };

uint16_t mtu;  // Maximum Transmission Unit
BLEService plantWateringService = BLEService(plantWateringServiceUuid);
BLECharacteristic thresholdCharacteristic = BLECharacteristic(thresholdCharacteristicUuid);
BLECharacteristic humidityNotifyCharacteristic = BLECharacteristic(humidityNotifyCharacteristicUuid);

// Pin definitions
int pump_pin = 11;
int led_pin = 13;
int sensor_pin = A0;

// Global variables
float threshold = 0.2;
uint16_t sensor_value;
bool is_waiting = false;
int counter = 0;
int waiting_time_s = 20;

void connectedCallback(uint16_t connHandle) {
  char centralName[32] = { 0 };
  BLEConnection* connection = Bluefruit.Connection(connHandle);
  connection->getPeerName(centralName, sizeof(centralName));
  Serial.print(connHandle);
  Serial.print(", connected to ");
  Serial.print(centralName);
  Serial.println();
}

void disconnectedCallback(uint16_t connHandle, uint8_t reason) {
  Serial.print(connHandle);
  Serial.print(" disconnected, reason = ");
  Serial.println(reason);  // see https://github.com/adafruit/Adafruit_nRF52_Arduino
  // /blob/master/cores/nRF5/nordic/softdevice/s140_nrf52_6.1.1_API/include/ble_hci.h
  Serial.println("Advertising ...");
}

void cccdCallback(uint16_t connHandle, BLECharacteristic* characteristic, uint16_t cccdValue) {
  if (characteristic->uuid == thresholdCharacteristic.uuid) {
    Serial.print("Humidity 'Notify', ");
    if (characteristic->notifyEnabled()) {
      Serial.println("enabled");
    } else {
      Serial.println("disabled");
    }
  }
}

int readIntegerFromUintArray(uint8_t* data, uint16_t len) {
  if (len == 2) {
    int d1 = data[0] << 8;
    int d2 = data[1] << 0;
    return d1 + d2;
  }
  Serial.println("Invalid data received.");
  return 0;
}

// Use uint16(little endian) on nrf app to write threshold
void writeCallback(uint16_t connHandle, BLECharacteristic* characteristic, uint8_t* Data, uint16_t len) {
  if (characteristic->uuid == thresholdCharacteristic.uuid) {
    threshold = (float)(Data[0] * 10 + Data[1]) / 1000;
    Serial.print("Threshold = ");
    Serial.print(threshold * 100);
    Serial.println("%");
  }
}

void setupService() {
  plantWateringService.begin();  // Must be called before calling .begin() on its characteristics

  thresholdCharacteristic.setProperties(CHR_PROPS_WRITE | CHR_PROPS_WRITE_WO_RESP);
  thresholdCharacteristic.setPermission(SECMODE_NO_ACCESS, SECMODE_OPEN);
  thresholdCharacteristic.setMaxLen(mtu);
  //thresholdCharacteristic.setFixedLen(2);
  thresholdCharacteristic.setWriteCallback(writeCallback, true);
  thresholdCharacteristic.begin();

  humidityNotifyCharacteristic.setProperties(CHR_PROPS_NOTIFY);
  humidityNotifyCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  //humidityNotifyCharacteristic.setMaxLen(mtu);
  humidityNotifyCharacteristic.setFixedLen(2);
  humidityNotifyCharacteristic.setCccdWriteCallback(cccdCallback);
  humidityNotifyCharacteristic.begin();
  uint8_t hrmData[2] = { 0b00000110, 0x40 };
  humidityNotifyCharacteristic.notify(hrmData, 2);
}

void startAdvertising() {
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addService(plantWateringService);
  Bluefruit.Advertising.addName();

  // See https://developer.apple.com/library/content/qa/qa1931/_index.html
  const int fastModeInterval = 32;   // * 0.625 ms = 20 ms
  const int slowModeInterval = 244;  // * 0.625 ms = 152.5 ms
  const int fastModeTimeout = 30;    // s
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(fastModeInterval, slowModeInterval);
  Bluefruit.Advertising.setFastTimeout(fastModeTimeout);
  // 0 = continue advertising after fast mode, until connected
  Bluefruit.Advertising.start(0);
  Serial.println("Advertising ...");
}

int writeIntToUintArray(int value, uint8_t data[]) {
  data[0] = value >> 8;
  data[1] = value >> 0;
  return value;
}

void setup() {
  // Pin setup
  pinMode(pump_pin, OUTPUT);
  pinMode(led_pin, OUTPUT);
  pinMode(sensor_pin, INPUT);

  digitalWrite(pump_pin, LOW);

  // Serial connection setup
  Serial.begin(115200);
  while (!Serial) { delay(10); }  // only if usb connected
  Serial.println("Setup");

  // BLE Setup
  Bluefruit.begin();
  Bluefruit.setName("nRF52840");
  Bluefruit.Periph.setConnectCallback(connectedCallback);
  Bluefruit.Periph.setDisconnectCallback(disconnectedCallback);

  deviceInfoService.setManufacturer("Adafruit Industries");
  deviceInfoService.setModel("Feather nRF52840 Sense");
  deviceInfoService.begin();

  batteryService.begin();
  batteryService.write(100);  // %

  mtu = Bluefruit.getMaxMtu(BLE_GAP_ROLE_PERIPH);

  setupService();
  startAdvertising();
}

void loop() {
  // Controll pump via button (only a test functionality)
  sensor_value = analogRead(sensor_pin);
  if (sensor_value < threshold * 1024) {
    if (!is_waiting) Serial.println("Moisture below threshold");
    Serial.print(((waiting_time_s) - counter));
    Serial.println(" s until watering the plant");
    is_waiting = true;
    counter++;
    if (counter >= (waiting_time_s)) {
      PumpWaterML(200);
      is_waiting = false;
      counter = 0;
    }
  } else {
    is_waiting = false;
    counter = 0;
  }
  if (Bluefruit.connected()) {
    uint8_t humData[2];
    writeIntToUintArray(sensor_value, humData);
    if (humidityNotifyCharacteristic.notify(humData, sizeof(humData))) {
      Serial.print("Moisture Measured: ");
      Serial.print((float)sensor_value/ 1024 * 100);
      Serial.println(" %");
    } else {
      Serial.println("Notify error.");
    }
  }
  delay(1000);  // ms
}

void PumpWaterML(int ml) {
  Serial.print("Watering plant with ");
  Serial.print(ml);
  Serial.println(" ml of water.");
  digitalWrite(led_pin, HIGH);
  digitalWrite(pump_pin, HIGH);
  delay(PUMP_MS_PER_ML * ml);
  digitalWrite(pump_pin, LOW);
  digitalWrite(led_pin, LOW);
}
