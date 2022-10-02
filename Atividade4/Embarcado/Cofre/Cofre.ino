#include "global.h"
#include <Servo.h>
Servo Motor;


void setup() {
  // put your setup code here, to run once:

  pinMode(CHANNEL_A, INPUT);
  pinMode(CHANNEL_B, INPUT);
  pinMode(CHANNEL_C, INPUT_PULLUP);
  pinMode(MOTOR_PWM, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(CHANNEL_A), ChannelA_irq, RISING);
  //attachInterrupt(digitalPinToInterrupt(CHANNEL_B), ChannelB_irq, RISING);

  Motor.attach(MOTOR_PWM);
  Serial.begin(9600);
  Motor.write(CLOSED);
}

void loop() {
  // put your main code here, to run repeatedly:

  checkRotation();

  if (checkChannelC()) {
    Serial.println("Pressed");
    password[passwordPos] = totalRotation;
    passwordPos++;
    if (passwordPos == 4) {
      if (checkPassword()) {
        Serial.println("foi");
        Motor.write(OPEN);
        password[0] = password[1] = password[2] = password[3] = 0;
        passwordPos = 0;
      } else {
        Serial.println("Não Foi");
        password[0] = password[1] = password[2] = password[3] = 0;
        passwordPos = 0;
      }
    }
  }


}

void ChannelA_irq() {

  actualCHANNEL_A = digitalRead(CHANNEL_A);

  if (!flagIRQ_B && !flagIRQ_A && !timeToCheck) {
    actualDirection = CW;
    flagToCheck = 1;
    flagIRQ_A = 1;

  }

  



}

void ChannelB_irq() {

  actualCHANNEL_B = digitalRead(CHANNEL_B);

  if (!flagIRQ_A && !flagIRQ_B && !timeToCheck) {
    actualDirection = CCW;
    flagToCheck = 1;
    flagIRQ_B = 1;

  }

  
}

unsigned char checkChannelC(void) {

  if (!digitalRead(CHANNEL_C)) {
    if (debounceButton) {
      debounceButton--;
      return 0;
    } else {
      debounceButton = DEBOUNCE_TICKS;
      if (!pressedFlag) {
        pressedFlag = 1;
        return 1;
      }

    }

  } else {
    pressedFlag = 0;
    debounceButton = DEBOUNCE_TICKS;
    return 0;
  }
}

void checkRotation(void) {
  if (flagIRQ_A) {
    flagIRQ_A = 0;
    //Serial.print("Canal A: ");
    //Serial.println(actualCHANNEL_A);
    timeToCheck = CHECK_ROTATION_TICKS;
  }

  if (flagIRQ_B) {
    flagIRQ_B = 0;
    //Serial.print("Canal B: ");
    //Serial.println(actualCHANNEL_B);
    timeToCheck = CHECK_ROTATION_TICKS;
  }

  if (!timeToCheck) {
    if (flagToCheck) {
      Serial.println("----------");

      if (actualDirection == CW) {
        totalRotation++;
      } else if (actualDirection == CCW) {
        totalRotation--;
      }
      flagToCheck = 0;
      flagIRQ_A = flagIRQ_B = 0;
      Serial.println(totalRotation);
    }

  } else {
    timeToCheck--;
  }
}

unsigned char checkPassword(void) {
  if (passwordDefault[0] == password[0] &&
      passwordDefault[1] == password[1] &&
      passwordDefault[2] == password[2] &&
      passwordDefault[3] == password[3]) {
    return 1;
  } else {
    return 0;
  }
}
