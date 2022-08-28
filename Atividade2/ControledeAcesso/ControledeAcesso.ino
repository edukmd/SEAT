#include <SPI.h>
#include <MFRC522.h>
#include "FUNCOES.h"
#include "globais.h"



#define RST_PIN         5          // Configurable, see typical pin layout above
#define SS_PIN          53         // Configurable, see typical pin layout above

#define BUZZER          22
#define LED_GREEN       23
#define BUTTON_REGISTER 24

MFRC522 rfid(SS_PIN, RST_PIN);  // Create MFRC522 instance

MFRC522::MIFARE_Key key;

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


  debounceCheck = 0;

  accessState = CHECK_USER;



}

void loop() {
  // put your main code here, to run repeatedly:

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
      if (checkUser(rfid.uid.uidByte)) {
        Serial.println("Usuario registrado. Acesso garantido!");
        accessState = ACCESS_CONTROL_GRANTED;

      } else {
        Serial.println("Usuario nao registrado. Acesso bloqueado!");
        accessState = ACCESS_CONTROL_BLOCKED;

      }
      break;
    case ACCESS_CONTROL_GRANTED:
      accessState = CHECK_USER;

      blinkLedBuzzer(100, 3);
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
  printHex(id, 4);
  printHex(defaultUser.tagNUID, 4);
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

void registerUser(unsigned char *id){
  User1.tagNUID[0] = id[0];
  User1.tagNUID[1] = id[1];
  User1.tagNUID[2] = id[2];
  User1.tagNUID[3] = id[3];
}
