#include <SPI.h>
#include <MFRC522.h>
#include "globais.h"



#define RST_PIN         5          // Configurable, see typical pin layout above
#define SS_PIN          53         // Configurable, see typical pin layout above

#define BUZZER          22
#define LED_GREEN       23
#define BUTTON_REGISTER 24
#define TRAVA_MAGNETICA 25

MFRC522 rfid(SS_PIN, RST_PIN);  // Create MFRC522 instance

MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;

int i = 0;

//FUNCTIONS
void printHex(unsigned char* data, unsigned char size_data);
unsigned char checkUser(unsigned char *id);
void blinkLedBuzzer(unsigned int msBlink, unsigned char times);
void registerUser(unsigned char *id);


user_t defaultUser{
  .name = "Usuario default",
  .tagNUID = {0x60, 0x4B, 0x0E, 0x30},
  .usedTimes = 0
};

user_t User1{
  .name = "Eduardo Kamada",
  .tagNUID = {0XFF, 0XFF, 0XFF, 0XFF},
  .usedTimes = 0
};



void setup() {
  // put your setup code here, to run once:

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  Serial.begin(9600);
  while (!Serial);
  SPI.begin();

  rfid.PCD_Init();
  delay(4);

  pinMode(BUZZER, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(BUTTON_REGISTER, INPUT_PULLUP);
  pinMode(TRAVA_MAGNETICA, OUTPUT);


  debounceCheck = 0;

  accessState = CHECK_USER;





}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned char size = sizeof(buffer);

  if (!digitalRead(BUTTON_REGISTER) && accessState == CHECK_USER) {
    accessState = REGISTER_USER;
    Serial.println("Aproxime o cartao para registrar novo usuario");
  }

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent() && (accessState == CHECK_USER || accessState == REGISTER_USER)) {
    return;
  }

  // Select one of the cards
  if ( ! rfid.PICC_ReadCardSerial() && (accessState == CHECK_USER || accessState == REGISTER_USER)) {
    return;
  }

  switch (accessState) {
    case CHECK_USER:

      //Autenticacao usando chave A
      status = rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                     TRAILER_ADDR_READTRIES, &key, &(rfid.uid));
      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(rfid.GetStatusCodeName(status));
        return;
      }
      status = rfid.MIFARE_Read(BLOCK_ADDR_READTRIES, buffer, &size);
      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(rfid.GetStatusCodeName(status));
      }

      accessOK = buffer[0];
      Serial.println(accessOK);

      if (checkUser(rfid.uid.uidByte)) {

        Serial.println("Acesso garantido!");
        accessState = ACCESS_CONTROL_GRANTED;

        status = rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B,
                                          BLOCK_ADDR_READTRIES, &key, &(rfid.uid));
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("PCD_Authenticate() failed: "));
          Serial.println(rfid.GetStatusCodeName(status));
          return;
        }
        accessOK++;
        buffer[0] = accessOK;

        //Grava no bloco 1
        status = rfid.MIFARE_Write(BLOCK_ADDR_READTRIES, buffer, 16);
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("MIFARE_Write() failed: "));
          Serial.println(rfid.GetStatusCodeName(status));
          return;
        }

      } else {
        Serial.println("Acesso bloqueado!");
        accessState = ACCESS_CONTROL_BLOCKED;

      }

      // Halt PICC
      rfid.PICC_HaltA();
      // Stop encryption on PCD
      rfid.PCD_StopCrypto1();
      break;
    case ACCESS_CONTROL_GRANTED:
      accessState = CHECK_USER;
      digitalWrite(TRAVA_MAGNETICA, HIGH);
      blinkLedBuzzer(100, 3);
      delay(2700);
      digitalWrite(TRAVA_MAGNETICA, LOW);


      break;
    case ACCESS_CONTROL_BLOCKED:
      accessState = CHECK_USER;

      blinkLedBuzzer(500, 1);
      break;
    case REGISTER_USER:
      if (checkUser(rfid.uid.uidByte)) {
        Serial.println("Usuario ja registrado. Tente Outro!");
        blinkLedBuzzer(500, 1);
      } else {
        registerUser(rfid.uid.uidByte);
        Serial.println("Usuario registrado com sucesso!");
        accessState = CHECK_USER;
        blinkLedBuzzer(100, 2);
        delay(1000);

      }
      break;
  }
}

void printHex(unsigned char* data, unsigned char size_data) {

  for (int i = 0; i < size_data; i++) {
    if (data[i] < 0xF) {
      Serial.print("0");
    }
    Serial.print(data[i], HEX);
    if (i != size_data - 1) {
      Serial.print(":");
    }

  }
  Serial.println("");

}

unsigned char checkUser(unsigned char *id) {
  //printHex(id, 4);
  //printHex(defaultUser.tagNUID, 4);
  if ((defaultUser.tagNUID[0] == id[0] && defaultUser.tagNUID[1] == id[1] && defaultUser.tagNUID[2] == id[2] && defaultUser.tagNUID[3] == id[3]) ||
      (User1.tagNUID[0] == id[0] && User1.tagNUID[1] == id[1] && User1.tagNUID[2] == id[2] && User1.tagNUID[3] == id[3])) {
    return 1;
  } else {
    return 0;
  }
}

void blinkLedBuzzer(unsigned int msBlink, unsigned char times) {
  unsigned char timeTmp = times * 2;
  for (unsigned char i = 0; i < timeTmp; i++) {
    digitalWrite(LED_GREEN, !digitalRead(LED_GREEN));
    digitalWrite(BUZZER, !digitalRead(BUZZER));
    delay(msBlink);
  }
}

void registerUser(unsigned char *id) {
  User1.tagNUID[0] = id[0];
  User1.tagNUID[1] = id[1];
  User1.tagNUID[2] = id[2];
  User1.tagNUID[3] = id[3];
}
