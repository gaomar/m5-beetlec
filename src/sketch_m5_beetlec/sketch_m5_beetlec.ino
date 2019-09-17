#include <M5StickC.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "carControl.h"

// Device Name: Maximum 30 bytes
#define DEVICE_NAME "M5-BeetleC"

// あなたのサービスUUIDを貼り付けてください
#define USER_SERVICE_UUID "119a679c-bb04-4798-8ff9-7ec96eb437ee"
// Notify UUID: トライアル版は値が固定される
#define NOTIFY_CHARACTERISTIC_UUID "62FBD229-6EDD-4D1A-B554-5C4E1BB29169"
// PSDI Service UUID: トライアル版は値が固定される
#define PSDI_SERVICE_UUID "E625601E-9E55-4597-A598-76018A0D293D"
// LIFFからのデータ UUID: トライアル版は値が固定される
#define WRITE_CHARACTERISTIC_UUID "E9062E71-9E62-4BC6-B0D3-35CDCD9B027B"
// PSDI CHARACTERISTIC UUID: トライアル版は値が固定される
#define PSDI_CHARACTERISTIC_UUID "26E2B12B-85F0-4F3F-9FDD-91D114270E6E"

BLEServer* thingsServer;
BLESecurity* thingsSecurity;
BLEService* userService;
BLEService* psdiService;
BLECharacteristic* psdiCharacteristic;
BLECharacteristic* notifyCharacteristic;
BLECharacteristic* writeCharacteristic;

bool deviceConnected = false;
bool oldDeviceConnected = false;

void BtnSet() {
  pinMode(37, INPUT_PULLUP);
}

class serverCallbacks: public BLEServerCallbacks {

  // デバイス接続
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;

  };

  // デバイス未接続
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;

  }
};

// LIFFから送信されるデータ
class writeCallback: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *bleWriteCharacteristic) {
    // LIFFから来るデータを取得
    std::string value = bleWriteCharacteristic->getValue();
    int myNum = atoi(value.c_str());
    
    leftwheel(myNum);
    rightwheel(myNum);
    
  }
};

void blink() {
  for(int num = 0; num < 7; num++) {
    uint32_t color = 0x11 << 16;
    led(num, color);
    delay(100);
    led(num, 0x00);
  }
  delay(100);
  for(int num = 0; num < 7; num++) {
    uint32_t color = 0x11 << 16;
    led(num, color);
    delay(100);
    led(num, 0x00);
  }
  delay(100);
  for(int num = 0; num < 7; num++) {
    uint32_t color = 0x11 << 16;
    led(num, color);
    delay(100);
    led(num, 0x00);
  }
}

void setup() {
  Serial.begin(115200);
  BLEDevice::init("");
  BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_NO_MITM);

  // Security Settings
  BLESecurity *thingsSecurity = new BLESecurity();
  thingsSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);
  thingsSecurity->setCapability(ESP_IO_CAP_NONE);
  thingsSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);

  setupServices();
  startAdvertising();

  // put your setup code here, to run once:
  // M5Stick LCD Setup
  M5.begin(true, false, false);
  Wire.begin(0, 26);
  
  // 横向き
  M5.Lcd.setRotation(3);
  
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextDatum(MC_DATUM);  
  M5.Lcd.drawString("Ready to Connect",80,10);
  Serial.println("Ready to Connect");

  BtnSet();
  blink();
}

void loop() {
  if (!deviceConnected && oldDeviceConnected) {
    delay(500); // Wait for BLE Stack to be ready
    thingsServer->startAdvertising(); // Restart advertising
    oldDeviceConnected = deviceConnected;
    Serial.println("Restart!");
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.setTextDatum(MC_DATUM);  
    M5.Lcd.drawString("Ready to Connect",80,10);
  }

  // Connection
  if (deviceConnected && !oldDeviceConnected) {
    oldDeviceConnected = deviceConnected;
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.setTextDatum(MC_DATUM);  
    M5.Lcd.drawString("Connect",80,10);
    
  }
  
}

// サービス初期化
void setupServices(void) {
  // Create BLE Server
  thingsServer = BLEDevice::createServer();
  thingsServer->setCallbacks(new serverCallbacks());

  // Setup User Service
  userService = thingsServer->createService(USER_SERVICE_UUID);

  // LIFFからのデータ受け取り設定
  writeCharacteristic = userService->createCharacteristic(WRITE_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE);
  writeCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
  writeCharacteristic->setCallbacks(new writeCallback());

  // Notifyセットアップ
  notifyCharacteristic = userService->createCharacteristic(NOTIFY_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_NOTIFY);
  notifyCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
  BLE2902* ble9202 = new BLE2902();
  ble9202->setNotifications(true);
  ble9202->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
  notifyCharacteristic->addDescriptor(ble9202);

  // Setup PSDI Service
  psdiService = thingsServer->createService(PSDI_SERVICE_UUID);
  psdiCharacteristic = psdiService->createCharacteristic(PSDI_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ);
  psdiCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);

  // Set PSDI (Product Specific Device ID) value
  uint64_t macAddress = ESP.getEfuseMac();
  psdiCharacteristic->setValue((uint8_t*) &macAddress, sizeof(macAddress));

  // Start BLE Services
  userService->start();
  psdiService->start();
}

void startAdvertising(void) {
  // Start Advertising
  BLEAdvertisementData scanResponseData = BLEAdvertisementData();
  scanResponseData.setFlags(0x06); // GENERAL_DISC_MODE 0x02 | BR_EDR_NOT_SUPPORTED 0x04
  scanResponseData.setName(DEVICE_NAME);

  thingsServer->getAdvertising()->addServiceUUID(userService->getUUID());
  thingsServer->getAdvertising()->setScanResponseData(scanResponseData);
  thingsServer->getAdvertising()->start();
}
