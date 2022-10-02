
#include "globais.h"

void rotateMotor(unsigned char sentido, unsigned char sequency);
unsigned int sensorTime(void);

void setup() {
  // put your setup code here, to run once:
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  digitalWrite(TRIGGER, LOW);
  tipo = FULL_STEP_SIMPLE;
  sentido_motor = HORARIO;

  flagCheckAngle = TIMER_CHECK_ANGLE;

  StateMachine = CHECK_DISTANCE;

  actualTime = millis();

  checkDistanceState = SEND_TRIGGER;

  Serial.begin(115200);

  stepsToReverseDirection = TOTAL_STEPS;

  actualAngle = 0;
  media_echo = 0;
  totalPoints = 0;

}

void loop() {
  // put your main code here, to run repeatedly:

  switch (StateMachine) {
    case ROTATE_MOTOR:

      //STEP motor every time Control time
      if (millis() - actualTime >= TIMER_CONTROL) {
        actualTime = millis();
        rotateMotor(sentido_motor, tipo);
        flagCheckAngle--;

        if (sentido_motor == HORARIO) {
          actualAngle += ANGLE_PER_STEP * 2;
        } else {
          actualAngle -= ANGLE_PER_STEP * 2;
        }

        if (actualAngle < 0) {
          actualAngle = 0;
        } else if (actualAngle > 180) {
          actualAngle = 180;
        }


        if (stepsToReverseDirection) {
          stepsToReverseDirection--;
        } else {
          stepsToReverseDirection = TOTAL_STEPS;

          if (sentido_motor == HORARIO) {
            sentido_motor = ANTI_HORARIO;
          } else {
            sentido_motor = HORARIO;
          }

        }
      }

      //Check HC-RCS04 sensor every TIMER_CONTROL * x time
      if (!flagCheckAngle) {
        StateMachine = CHECK_DISTANCE;

        flagCheckAngle = TIMER_CHECK_ANGLE;

        Serial.print(actualAngle);
        Serial.print(',');
        Serial.println(dist_eq1);
      }

      break;

    case CHECK_DISTANCE:

      echoTime = sensorTime();
      dist_eq1 = 0.0177 * echoTime - 0.6945;

      StateMachine = ROTATE_MOTOR;

      break;

    case SEND_DATA:

      //Serial.print(actualAngle);
      //Serial.print(',');
      //Serial.println(dist_eq1);

      StateMachine = ROTATE_MOTOR;
      break;
  }



}

void rotateMotor(unsigned char sentido, unsigned char sequency) {
  static char pos = 0;



  switch (sequency) {
    case FULL_STEP_SIMPLE:

      if (!sentido) {
        pos++;
        if (pos > 7) {
          pos = 0;
        }
      } else {
        pos--;
        if (pos < 0) {
          pos = 7;
        }
      }

      switch (pos) {

        case 0:
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
          break;

        case 1:
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, LOW);
          break;

        case 2:
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
          break;

        case 3:
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, HIGH);
          break;

        case 4:
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
          break;

        case 5:
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, LOW);
          break;

        case 6:
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
          break;

        case 7:
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, HIGH);
          break;
      }

      break;


    case FULL_STEP_DOUBLE:

      if (!sentido) {
        pos++;
        if (pos > 15) {
          pos = 0;
        }
      } else {
        pos--;
        if (pos < 0) {
          pos = 15;
        }
      }

      switch (pos) {

        case 0:
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, LOW);
          break;

        case 1:
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, LOW);
          break;

        case 2:
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, LOW);
          break;

        case 3:
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
          break;

        case 4:
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
          break;

        case 5:
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, HIGH);
          break;

        case 6:
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
          break;

        case 7:
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
          break;

        case 8:
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, LOW);
          break;

        case 9:
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, LOW);
          break;

        case 10:
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, LOW);
          break;

        case 11:
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
          break;

        case 12:
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
          break;

        case 13:
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, HIGH);
          break;

        case 14:
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
          break;

        case 15:
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
          break;
      }


      break;
  }
}

unsigned int sensorTime(void) {
  //Pulso de 10us
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);
  //Lê tempo atual
  previousMicros = micros();
  //Aguarda borda de subida ou timeout
  while (!digitalRead(ECHO) && (micros() - previousMicros) <= TIMEOUT_SENSOR); // wait for the echo pin HIGH or timeout
  //Atualiza o tempo atual
  previousMicros = micros();
  //Aguarda borda de descida ou timeout
  while (digitalRead(ECHO)  && (micros() - previousMicros) <= TIMEOUT_SENSOR); // wait for the echo pin LOW or timeout
  //Calcula a diferença de tempos

  return micros() - previousMicros; // duration
}
