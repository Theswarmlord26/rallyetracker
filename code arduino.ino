#include <ArduinoBLE.h>
#include <Arduino_LSM6DS3.h>
#include <stdlib.h>

String address;

BLEService uartService("6E400001-B5A3-F393-E0A9-E50E24DCCA9E"); // UART service UUID
BLECharacteristic rxChar("6E400002-B5A3-F393-E0A9-E50E24DCCA9E", BLEWriteWithoutResponse, 20); // RX characteristic UUID
BLECharacteristic txChar("6E400003-B5A3-F393-E0A9-E50E24DCCA9E", BLERead | BLENotify, 20); // TX characteristic UUID

bool reconnaissanceMode = false;

void rxCharWritten(BLEDevice central, BLECharacteristic rxChar) {
  const uint8_t* rxValue = rxChar.value();
  int rxLength = rxChar.valueLength();

  BLECharacteristic txCharRef = uartService.characteristic(6); // Use the characteristic index to get a reference to the object
  uint8_t txValue[rxLength];

  memcpy(txValue, rxValue, rxLength);

  txCharRef.writeValue(txValue, rxLength);

  String rxStr((char*)rxValue);
  Serial.print("Received data from central: ");
  Serial.println(rxStr);

  // Si la commande est 3, envoyer l'adresse Bluetooth de l'appareil
  if (*rxValue == '3') {
    Serial.print("Sending local Bluetooth address: ");
    Serial.println(address);

    // Convertir la chaîne de caractères en tableau d'octets
    uint8_t addressBytes[address.length()];
    address.getBytes(addressBytes, address.length());

    // Envoyer l'adresse Bluetooth via la caractéristique TX
    txChar.writeValue(addressBytes, address.length());
  }
  // Si la commande est 4, passer en mode rallye et attendre les informations du Bluetooth
  else if (*rxValue == '4') {
    Serial.println("Switching to rally mode...");
    reconnaissanceMode = false;
  }
  // Si la commande est 5, passer en mode reconnaissance et exécuter le programme existant
  else if (*rxValue == '5') {
    Serial.println("Switching to reconnaissance mode...");
    reconnaissanceMode = true;
  }
}

void sendGyroData() {
  float gx, gy, gz;
  IMU.readGyroscope(gx, gy, gz);

  // Conversion des valeurs de gyroscope en entiers
  int16_t gx_int = 0; // Gyro X
  int16_t gy_int = (int16_t)(gy * 1); // Gyro Y
  int16_t gz_int = 0; // Gyro Z

  // Limitation des valeurs à -360 et 360
  gy_int = max(min(gy_int, 360), -360);

  uint8_t gyroData[2];
  
  // Copier la valeur d'entier dans le tableau d'octets
  memcpy(&gyroData[0], &gy_int, sizeof(gy_int));

  // Convertir les données binaires en chaîne hexadécimale
  char hexStr[7];
  sprintf(hexStr, "Y:%d", gy_int);

  // Envoyer la chaîne hexadécimale via Bluetooth
  txChar.writeValue(hexStr);
}

void sendAccelDataKmh() {
    float ax, ay, az;
    IMU.readAcceleration(ax, ay, az);

    // Conversion des valeurs d'accéléromètre en km/h
    float kmh_x = ax * 3.6;

    // Convertir les données en chaîne de caractères
    char accelData[50];
    sprintf(accelData, "X:%.2f km/h", kmh_x);

    // Envoyer la chaîne de caractères via Bluetooth
    txChar.writeValue(accelData);
}
void measureAngle() {
  float ax, ay, az;
  IMU.readAcceleration(ax, ay, az);

  //Calcul de l'angle d'inclinaison en degrés
  float roll = atan2(-ay, az) * RAD_TO_DEG;
  float pitch = atan2(ax, sqrt(ay * ay + az * az)) * RAD_TO_DEG;

  Serial.print("Roll: ");
  Serial.print(roll);
  Serial.print(" Pitch: ");
  Serial.println(pitch);

  // Envoyer la mesure d'angle via Bluetooth
  char pitchData[20];
  sprintf(pitchData, "%.2f", pitch);
  txChar.writeValue(pitchData);
}

void setup() {
  Serial.begin(9600);
  
  while (!Serial);

  if (!BLE.begin()) {
    Serial.println("Failed to initialize BLE!");
    while (1);
  }
  address = BLE.address();
  Serial.print("local adress is ");
  Serial.println(address);
  BLE.setLocalName("Prog Rallye");
  BLE.setDeviceName("Prog Rallye");
  if (!IMU.begin()) {
    Serial.println("Failed to initialize LSM6DS3!");
    while (1);

  }

  // Ajouter le service et les caractéristiques Bluetooth
  uartService.addCharacteristic(rxChar);
  uartService.addCharacteristic(txChar);
  BLE.addService(uartService);

  // Définir la fonction de rappel pour la caractéristique RX
  rxChar.setEventHandler(BLEWritten, rxCharWritten);

  // Commencer la publicité Bluetooth
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // Vérifier s'il y a une connexion Bluetooth active
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
  }

  // Tant que la connexion est active
  while (central.connected()) {
    if (!reconnaissanceMode) {
      if (rxChar.written()) {
        const uint8_t* rxValue = rxChar.value();
        int rxLength = rxChar.valueLength();

        // Si la commande est 1, envoyer les données du gyroscope
        if (*rxValue == '1') {
          for (int i = 0; i < 1; i++) {
            //sendGyroData();
            measureAngle();
            delay(100);
          }
        }
        
        // Si la commande est 2, envoyer les données de l'accéléromètre
      else if (*rxValue == '2') {
        for (int i = 0; i < 1; i++) {
        sendAccelDataKmh();
        delay(100);
    }
}
      }
    }
  }
}