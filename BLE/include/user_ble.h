#ifndef USER_BLE_H
#define USER_BLE_H
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

typedef void (*user_ble_characteristic_t)(String str); 
user_ble_characteristic_t ble_callback = NULL; 
String bleConvertStdStringToArduinoString(std::string str); 

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define PASSKEY 160299


static bool bleAuthenticationFlag = false; 
static bool deviceConnceted = false; 
static bool oldDeviceConnected = false; 

void bleSetUserCallback(void * cb)
{
  ble_callback = (user_ble_characteristic_t)cb;
}

class ServerCallback: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println(" - ServerCallback - onConnect");
      deviceConnceted = true; 
    };

    void onDisconnect(BLEServer* pServer) {
      Serial.println(" - ServerCallback - onDisconnect");
      deviceConnceted = false; 
    }
};

class SecurityCallback : public BLESecurityCallbacks {

  uint32_t onPassKeyRequest(){
    return 000000;
  }

  void onPassKeyNotify(uint32_t pass_key){}

  bool onConfirmPIN(uint32_t pass_key){
    vTaskDelay(5000);
    return true;
  }

  bool onSecurityRequest(){
    return true;
  }

  void onAuthenticationComplete(esp_ble_auth_cmpl_t cmpl){
    if(cmpl.success){
      Serial.println("   - SecurityCallback - Authentication Success");       
    }else{
      Serial.println("   - SecurityCallback - Authentication Failure*");
      pServer->removePeerDevice(pServer->getConnId(), true);
    }
    BLEDevice::startAdvertising();
  }
};

class BLECallback : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string value = pCharacteristic->getValue(); 
        if(value.length() > 0)
        {
            String strValue = bleConvertStdStringToArduinoString(value); 
            Serial.println("Data nhan duoc la: " + strValue); 
            ble_callback(strValue); 
        }
    }
}; 



void bleSecurity(){
  esp_ble_auth_req_t auth_req = ESP_LE_AUTH_REQ_SC_MITM_BOND;
  esp_ble_io_cap_t iocap = ESP_IO_CAP_OUT;          
  uint8_t key_size = 16;     
  uint8_t init_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
  uint8_t rsp_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
  uint32_t passkey = PASSKEY;
  uint8_t auth_option = ESP_BLE_ONLY_ACCEPT_SPECIFIED_AUTH_DISABLE;
  esp_ble_gap_set_security_param(ESP_BLE_SM_SET_STATIC_PASSKEY, &passkey, sizeof(uint32_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_AUTHEN_REQ_MODE, &auth_req, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_IOCAP_MODE, &iocap, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_MAX_KEY_SIZE, &key_size, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_ONLY_ACCEPT_SPECIFIED_SEC_AUTH, &auth_option, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_SET_INIT_KEY, &init_key, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_SET_RSP_KEY, &rsp_key, sizeof(uint8_t));
}

void bleInit(){
  BLEDevice::init("TestBLE");
  BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);
  BLEDevice::setSecurityCallbacks(new SecurityCallback());

  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallback());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY | 
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  pCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
  pCharacteristic->setCallbacks(new BLECallback()); 
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();

  // Start BLE Advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);
  BLEDevice::startAdvertising();

  bleSecurity();
}

String bleConvertStdStringToArduinoString(std::string str)
{
    String objString; 
    for(int i = 0; i < str.length(); i++)
    {
        objString += (char)str[i]; 
    }
    return objString; 
}

std::string bleConvertArduinoStringToStdString(String str)
{
  std::string objstring; 
  for(int i = 0; i < str.length(); i++)
  {
    objstring += (char)str[i]; 
  }
  return objstring; 
}

void bleHandle()
{
  if(!deviceConnceted && oldDeviceConnected)
  {
    vTaskDelay(500 / portTICK_PERIOD_MS);
    pServer->startAdvertising(); 
    oldDeviceConnected = deviceConnceted; 
  }
  if(deviceConnceted && !oldDeviceConnected)
  {
    oldDeviceConnected = deviceConnceted; 
  }
}

void bleSendNotify(String notifyConten)
{
  if(deviceConnceted)
  {
    std::string str =  bleConvertArduinoStringToStdString(notifyConten); 
    pCharacteristic->setValue(str); 
    pCharacteristic->notify(); 
  }
}

#endif
