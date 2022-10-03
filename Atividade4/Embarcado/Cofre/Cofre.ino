#include "global.h"
#include "mario_song.h"
#include <Servo.h>
Servo Motor;





void setup() {
  // put your setup code here, to run once:

  pinMode(CHANNEL_A, INPUT);
  pinMode(CHANNEL_B, INPUT);
  pinMode(CHANNEL_C, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(MOTOR_PWM, OUTPUT);

  pinMode(melodyPin, OUTPUT);//buzzer


  attachInterrupt(digitalPinToInterrupt(CHANNEL_A), Channel_irq, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CHANNEL_B), Channel_irq, CHANGE);

  digitalWrite(GREEN_LED, LOW);

  Motor.attach(MOTOR_PWM);
  Serial.begin(115200);
  Motor.write(CLOSE);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (passwordPassed) {
    doorValue = analogRead(DOOR_CHECK);
    

    if (doorValue > 500 && !checkDoorOpen) {
      if (timeToOpen) {
        timeToOpen--;
      } else {
        timeToOpen = DEBOUNCE_TO_OPEN;
        checkDoorOpen = 1;
        Serial.println("Porta Aberta");
        sing(MARIO_THEME);
      }

    } else if (doorValue < 400  && checkDoorOpen) {
      if (timeToClose) {
        timeToClose--;
      } else {
        timeToClose = DEBOUNCE_TO_CLOSE;
        checkDoorOpen = 0;
        Motor.write(CLOSE);
        passwordPassed = 0;
        digitalWrite(GREEN_LED, LOW);
        Serial.println("Porta Fechada");
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
        Serial.println("Destravado");
        passwordPassed = 1;
        Motor.write(OPEN);
        password[0] = password[1] = password[2] = password[3] = 0;
        passwordPos = 0;
        digitalWrite(GREEN_LED, HIGH);

        
      } else {
        passwordPassed = 0;
        Serial.println("Senha incorreta");
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


void sing(int s) {
  // iterate over the notes of the melody:
  song = s;
  if (song == 2) {
    Serial.println(" 'Underworld Theme'");
    int size = sizeof(underworld_melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / underworld_tempo[thisNote];

      buzz(melodyPin, underworld_melody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      buzz(melodyPin, 0, noteDuration);

    }

  } else {

    Serial.println(" 'Mario Theme'");
    int size = sizeof(melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / tempo[thisNote];

      buzz(melodyPin, melody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      buzz(melodyPin, 0, noteDuration);

    }
  }
}

void buzz(int targetPin, long frequency, long length) {
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }

}
