#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         5          // Configurable, see typical pin layout above
#define SS_PIN          53         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

MFRC522::MIFARE_Key key;

// Init array that will store new NUID
byte nuidPICC[4];

void setup() {
  // put your setup code here, to run once:

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  Serial.begin(9600);
  while (!Serial);
  SPI.begin();

  mfrc522.PCD_Init();
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();


  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

}

void loop() {
  // put your main code here, to run repeatedly:

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the card; PICC_HaltA() is automatically called
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

}
