/*
  -----------------------------------------------------------------------------------------------
 | BLE_IMU_PERIPHERAL - Wireless IMU Communication with central device
 |
 | Arduino Boards Tested: Nano 33 BLE Sense as a peripheral & Nano 33 BLE as central.
 | Code not tested for multiple peripherals

 | This sketch works alongside the BLE_IMU_CENTRAL sketch to communicate with an Arduino Nano 33 BLE. 
 | This sketch can also be used with a generic BLE central app, like LightBlue (iOS and Android) or
 | nRF Connect (Android), to interact with the services and characteristics created in this sketch.
 
 | This example code is adapted from the ArduinoBLE library, available in the public domain.
 | Authors: Aaron Yurkewich & Pilar Zhang Qiu
 | Latest Update: 25/02/2021
  -----------------------------------------------------------------------------------------------
*/

#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

// ------------------------------------------ BLE UUIDs ------------------------------------------
#define BLE_UUID_PERIPHERAL               "19B10000-E8F2-537E-4F6C-D104768A1214" //please chnage to a unique value that matches BLE_IMU_CENTRAL
#define BLE_UUID_CHARACT_LED              "19B10001-E8F2-537E-4F6C-E104768A1214" //please chnage to a unique value that matches BLE_IMU_CENTRAL
#define BLE_UUID_CHARACT_ACCX             "29B10001-E8F2-537E-4F6C-a204768A1215" //please chnage to a unique value that matches BLE_IMU_CENTRAL
#define BLE_UUID_CHARACT_ACCY             "39B10001-E8F2-537E-4F6C-a204768A1215" //please chnage to a unique value that matches BLE_IMU_CENTRAL
#define BLE_UUID_CHARACT_ACCZ             "49B10001-E8F2-537E-4F6C-a204768A1215" //please chnage to a unique value that matches BLE_IMU_CENTRAL

BLEService LED_IMU_Service(BLE_UUID_PERIPHERAL); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic(BLE_UUID_CHARACT_LED, BLERead | BLEWrite);
BLEFloatCharacteristic accXCharacteristic(BLE_UUID_CHARACT_ACCX, BLERead | BLENotify | BLEWrite);
BLEFloatCharacteristic accYCharacteristic(BLE_UUID_CHARACT_ACCY, BLERead | BLENotify | BLEWrite);
BLEFloatCharacteristic accZCharacteristic(BLE_UUID_CHARACT_ACCZ, BLERead | BLENotify | BLEWrite);


const int ledPin = LED_BUILTIN; // pin to use for the LED
float x, y, z;

// ------------------------------------------ VOID SETUP ------------------------------------------
void setup() {
  Serial.begin(9600); //remove 'Serial' lines of code if you do not want to have to run the serial monitor
  while (!Serial); 

  // begin IMU initialization
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  // set LED pin to output mode
  pinMode(ledPin, OUTPUT);

  // begin BLE initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("BLE_IMU");
  BLE.setAdvertisedService(LED_IMU_Service);

  // add the characteristic to the service
  LED_IMU_Service.addCharacteristic(switchCharacteristic);
  LED_IMU_Service.addCharacteristic(accXCharacteristic);
  LED_IMU_Service.addCharacteristic(accYCharacteristic);
  LED_IMU_Service.addCharacteristic(accZCharacteristic);


  // add service
  BLE.addService(LED_IMU_Service);

  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0);


  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");
}

// ------------------------------------------ VOID LOOP ------------------------------------------
void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();


  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (switchCharacteristic.written()) {
        if (switchCharacteristic.value()) {   // any value other than 0
          Serial.println("LED on");
          digitalWrite(ledPin, HIGH);         // will turn the LED on
        } else {                              // a 0 value
          Serial.println(F("LED off"));
          digitalWrite(ledPin, LOW);          // will turn the LED off
        }
      }

      if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(x, y, z);

        accXCharacteristic.writeValue(x);
        accYCharacteristic.writeValue(y);
        accZCharacteristic.writeValue(z);

        Serial.print(x); 
        Serial.print('\t');
        Serial.print(y); 
        Serial.print('\t');         
        Serial.print(z); 
        Serial.println('\t'); 
      }

    }

      // when the central disconnects, print it out:
      Serial.print(F("Disconnected from central: "));
      Serial.println(central.address());
    }
  }
