#include "global.h"
#include <Servo.h>
Servo Motor;


void setup() {
  // put your setup code here, to run once:

  pinMode(CHANNEL_A, INPUT);
  pinMode(CHANNEL_B, INPUT);
  pinMode(CHANNEL_C, INPUT_PULLUP);
  pinMode(GREEN_LED,OUTPUT);
  pinMode(MOTOR_PWM, OUTPUT);
  

  attachInterrupt(digitalPinToInterrupt(CHANNEL_A), Channel_irq, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CHANNEL_B), Channel_irq, CHANGE);

  digitalWrite(GREEN_LED,LOW);

  Motor.attach(MOTOR_PWM);
  Serial.begin(115200);
  Motor.write(CLOSE);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (passwordPassed) {
    doorValue = analogRead(DOOR_CHECK);

    if (doorValue > 500 && !checkDoorOpen) {
      checkDoorOpen = 1;
    } else if (doorValue < 500  && checkDoorOpen) {
      if (timeToClose) {
        timeToClose--;
      } else {
        timeToClose = DEBOUNCE_TO_CLOSE;
        checkDoorOpen = 0;
        Motor.write(CLOSE);
        passwordPassed = 0;
        digitalWrite(GREEN_LED,LOW);
      }

    }
  }



  //Serial.println(doorValue);

  checkRotation();

  if (checkChannelC()) {
    Serial.println("Pressed");
    password[passwordPos] = totalRotation;
    passwordPos++;
    if (passwordPos == 4) {
      if (checkPassword()) {
        Serial.println("foi");
        passwordPassed = 1;
        Motor.write(OPEN);
        password[0] = password[1] = password[2] = password[3] = 0;
        passwordPos = 0;
        digitalWrite(GREEN_LED,HIGH);
      } else {
        passwordPassed = 0;
        Serial.println("Não Foi");
        password[0] = password[1] = password[2] = password[3] = 0;
        passwordPos = 0;
      }
    }
  }


}

void Channel_irq() {
  lastCHANNEL_A = actualCHANNEL_A;
  lastCHANNEL_B = actualCHANNEL_B;
  actualCHANNEL_A = digitalRead(CHANNEL_A);
  actualCHANNEL_B = digitalRead(CHANNEL_B);

  if (actualCHANNEL_A != lastCHANNEL_A) {

    if (!actualCHANNEL_A) {

      if (!lastCHANNEL_B && !actualCHANNEL_B) {
        actualDirection = CW;
      } else if (lastCHANNEL_B && actualCHANNEL_B) {
        actualDirection = CCW;
      }


    } else if (actualCHANNEL_A) {
      if (lastCHANNEL_B && actualCHANNEL_B) {
        actualDirection = CW;
      } else if (!lastCHANNEL_B && !actualCHANNEL_B) {
        actualDirection = CCW;
      }
    }

  } else if (actualCHANNEL_B != lastCHANNEL_B) {

    if (!actualCHANNEL_B) {
      if (lastCHANNEL_A && actualCHANNEL_A) {
        actualDirection = CW;
      } else if (!lastCHANNEL_A && !actualCHANNEL_A) {
        actualDirection = CCW;
      }

    } else {
      if (!lastCHANNEL_A && !actualCHANNEL_A) {
        actualDirection = CW;
      } else if (lastCHANNEL_A && actualCHANNEL_A) {
        actualDirection = CCW;
      }
    }


  }

  flagToCheck++;
  timeToCheck = CHECK_ROTATION_TICKS;

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

  if (flagToCheck > 3) {
    if (timeToCheck) {
      timeToCheck--;
    } else {
      flagToCheck = 0;
      if (actualDirection == CW) {
        totalRotation++;
      } else {
        totalRotation--;
      }

      Serial.println(totalRotation);

    }
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
