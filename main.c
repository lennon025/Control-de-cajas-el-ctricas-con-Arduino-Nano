#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>

SoftwareSerial BTSerial(2,3); // terminales RX y TX
String clave = "";// String para almacenar el número recibido
//led verde encedido, led rojo apagado.
int LedVerde = 4;
int LedRojo = 5;
int rele = 6;
int dt = 50;
int contador = 0;
int contadorc =0;
const int oneWirePin = 7;
int fan = 8;

OneWire oneWireBus(oneWirePin);
DallasTemperature sensor(&oneWireBus); // temperatura
unsigned long currentTime = millis(); 

float temperature;

const int RST_PIN = 9;        // Pin 9 para el reset del RC522 // MOSI - D11// MISO - D12 
const int SS_PIN = 10;        // Pin 10 para el SS (SDA) del RC522 // SCK - D13
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Crear instancia del MFRC522

// claves valida
byte validKey1[4] = { 0xE3, 0x89, 0x94, 0x13 }; // введите действительный код магнитного ключа. 
byte validKey2[4] = { 0xAD, 0x0E, 0x22, 0xA4 };
byte validKey3[4] = { 0x9D, 0x02, 0x1D, 0xA4 };
byte validKey4[4] = { 0x9D, 0x03, 0x1D, 0xA4 };
byte validKey5[4] = { 0xCD, 0xD2, 0x16, 0xA4 };
byte validKey6[4] = { 0xDD, 0x9D, 0x28, 0xA4 };
byte validKey7[4] = { 0x8D, 0x5D, 0x15, 0xA4 };
byte validKey8[4] = { 0xAD, 0x85, 0x1A, 0xA4 };
byte validKey9[4] = { 0xDD, 0xB1, 0x21, 0xA4 };
byte validKey10[4] = { 0xCD, 0x58, 0x1A, 0xA4 };
byte validKey11[4] = { 0x3D, 0xC8, 0x27, 0xA4 };

//byte validKey2[4] = { 0x83, 0xB6, 0x1D, 0x2A };// para agregar mas llaves

//
bool isEqualArray(byte* arrayA, byte* arrayB, int length)
{
  for (int index = 0; index < length; index++)
  {
    if (arrayA[index] != arrayB[index]) return false;
  }
  return true;
}

void printArray(byte *buffer, byte bufferSize) {
   for (byte i = 0; i < bufferSize; i++) {
      Serial.print(buffer[i] < 0x10 ? " 0" : " ");
      Serial.print(buffer[i], HEX);
   }
}
 
void setup()
{
   Serial.begin(9600);      //Inicializa la velocidad de Serial
   BTSerial.begin(9600);
   SPI.begin();         //Función que inicializa SPI
   mfrc522.PCD_Init();     //Función  que inicializa RFID
   pinMode(LedVerde, OUTPUT);
   pinMode(LedRojo, OUTPUT);
   pinMode(rele, OUTPUT);
   pinMode(fan, OUTPUT);
   sensor.begin();
}
 
void loop() {
  sensor.requestTemperatures();
  //Serial.print("Temperatura en sensor 0: ");
  //Serial.print(sensor.getTempCByIndex(0));
  //Serial.println(" ºC");
  if (sensor.getTempCByIndex(0) >= 28){
    digitalWrite(fan,HIGH);
    }
  if (sensor.getTempCByIndex(0) <= 27){
    digitalWrite(fan,LOW);
    }

  // Detectar tarjeta
  if (mfrc522.PICC_IsNewCardPresent()){
    //Seleccionamos una tarjeta
    if (mfrc522.PICC_ReadCardSerial()){
         Serial.print(F("Card UID:"));
         printArray(mfrc522.uid.uidByte, mfrc522.uid.size);
         Serial.println();
 
         // Finalizar lectura actual
         mfrc522.PICC_HaltA();

      // Comparar ID con las claves válidas
     /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
       //if (isEqualArray(mfrc522.uid.uidByte, validKey1, 4)){ 
        //contador, para apagar y encender los componentes con una unica llave. 
       if (isEqualArray(mfrc522.uid.uidByte, validKey1, 4) 
       || isEqualArray(mfrc522.uid.uidByte, validKey2, 4)
       || isEqualArray(mfrc522.uid.uidByte, validKey3, 4)
       || isEqualArray(mfrc522.uid.uidByte, validKey4, 4)
       || isEqualArray(mfrc522.uid.uidByte, validKey5, 4)
       || isEqualArray(mfrc522.uid.uidByte, validKey6, 4)
       || isEqualArray(mfrc522.uid.uidByte, validKey7, 4)
       || isEqualArray(mfrc522.uid.uidByte, validKey8, 4)
       || isEqualArray(mfrc522.uid.uidByte, validKey9, 4)
       || isEqualArray(mfrc522.uid.uidByte, validKey10, 4)
       || isEqualArray(mfrc522.uid.uidByte, validKey11, 4)){
        contador = contador + 1;
        delay(200);
       }
        switch(contador){
          case 1:
            Serial.println("Encender");
            digitalWrite(LedRojo,LOW);
            digitalWrite(LedVerde,HIGH);
            digitalWrite(rele,HIGH);
          break;

          case 2:
            Serial.println("Apagar");
            digitalWrite(LedRojo,HIGH);
            digitalWrite(LedVerde,LOW);
            digitalWrite(rele,LOW);
            contador = 0;
          break;
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
      // Finalizar lectura actual
      mfrc522.PICC_HaltA();
    }
  }
  //Conexion inalambrica.
 if (BTSerial.available()) {
   char Mensaje = BTSerial.read();
   if (Mensaje >= '0' && Mensaje <= '9') {
     clave += Mensaje;
   }
    else if (Mensaje == '\n') {
		//введите действительный код bluetooth.
        if (clave == "55555") { 
          contadorc = contadorc + 1;
          delay(200);
        switch(contadorc){
          case 1:
            Serial.println("Encender");
            digitalWrite(LedRojo,LOW);
            digitalWrite(LedVerde,HIGH);
            digitalWrite(rele,HIGH);
          break;

          case 2:
            Serial.println("Apagar");
            digitalWrite(LedRojo,HIGH);
            digitalWrite(LedVerde,LOW);
            digitalWrite(rele,LOW);
            contadorc = 0;
          break;
        }
      }
      clave = ""; // Reinicia el string para recibir un nuevo número
    } 
  }
    delay(500);
 }
