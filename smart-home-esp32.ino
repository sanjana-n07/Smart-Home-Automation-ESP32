#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define LED1 25
#define LED2 26
#define LED3 27
#define LED4 14

#define BUTTON1 18
#define BUTTON2 19
#define BUTTON3 21
#define BUTTON4 22

bool led1State = false;
bool led2State = false;
bool led3State = false;
bool led4State = false;

bool lastB1 = HIGH;
bool lastB2 = HIGH;
bool lastB3 = HIGH;
bool lastB4 = HIGH;

// BLE UUIDs
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic *pCharacteristic;

void updateLEDs() {
  digitalWrite(LED1, led1State);
  digitalWrite(LED2, led2State);
  digitalWrite(LED3, led3State);
  digitalWrite(LED4, led4State);
}

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {

    String command = pCharacteristic->getValue();

    if (command.length() == 0) return;

    Serial.print("Received: ");
    Serial.println(command);

    if (command == "L1ON") led1State = true;
    if (command == "L1OFF") led1State = false;

    if (command == "L2ON") led2State = true;
    if (command == "L2OFF") led2State = false;

    if (command == "L3ON") led3State = true;
    if (command == "L3OFF") led3State = false;

    if (command == "L4ON") led4State = true;
    if (command == "L4OFF") led4State = false;

    updateLEDs();
  }
};

void setup() {

  Serial.begin(115200);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);

  updateLEDs();

  // Start Bluetooth
  BLEDevice::init("ESP32_HOME");

  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService =
      pServer->createService(SERVICE_UUID);

  pCharacteristic =
      pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE
      );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("Ready");

  pService->start();

  BLEAdvertising *pAdvertising =
      BLEDevice::getAdvertising();

  pAdvertising->addServiceUUID(SERVICE_UUID);

  BLEDevice::startAdvertising();

  Serial.println("ESP32 HOME AUTOMATION READY");
  Serial.println("Bluetooth name: ESP32_HOME");
}

void loop() {

  bool b1 = digitalRead(BUTTON1);
  bool b2 = digitalRead(BUTTON2);
  bool b3 = digitalRead(BUTTON3);
  bool b4 = digitalRead(BUTTON4);

  // Button 1
  if (lastB1 == HIGH && b1 == LOW) {
    led1State = !led1State;
    updateLEDs();
    Serial.println("Button 1");
    delay(150);
  }

  // Button 2
  if (lastB2 == HIGH && b2 == LOW) {
    led2State = !led2State;
    updateLEDs();
    Serial.println("Button 2");
    delay(150);
  }

  // Button 3
  if (lastB3 == HIGH && b3 == LOW) {
    led3State = !led3State;
    updateLEDs();
    Serial.println("Button 3");
    delay(150);
  }

  // Button 4
  if (lastB4 == HIGH && b4 == LOW) {
    led4State = !led4State;
    updateLEDs();
    Serial.println("Button 4");
    delay(150);
  }

  lastB1 = b1;
  lastB2 = b2;
  lastB3 = b3;
  lastB4 = b4;
}