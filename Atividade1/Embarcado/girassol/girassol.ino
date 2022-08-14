#include <Servo.h>
Servo Motor;

//DEFINES
#define VALOR_DEBOUNCE 10
#define CALIBRATION_DEBOUNCE_CYCLE 2000
#define ESQUERDA 0
#define DIREITA 172
#define CENTRO 84

enum STATE {
  CALIBRATION_LEFT_LDR,
  CALIBRATION_RIGHT_LDR,
  NORMAL_OPERATION,
  CHECK_CALIBRATION

};

//VARIÁVEIS
int leftLdr = 0;
int rightLdr = 0;
int angle = 90;

int leftOffset = 0;
int rightOffset = 0;

int leftLdrReal = 0;
int rightLdrReal = 0;
int printserial = 100;
int debounce_angle = 5;
int calibrationDebounce = CALIBRATION_DEBOUNCE_CYCLE;

int leftLdrMaxValue = 0;
int rightLdrMaxValue = 0;

int leftLdrMinValue = 0;
int rightLdrMinValue = 0;

//Protótipo
void getLdrValues(void);
void setLdrRealValues(void);
int getMotorAngle(void);
void setMotorAngle(int angle);
void loopingControl(void);



unsigned char machineState = CALIBRATION_LEFT_LDR;



void setup() {
  // put your setup code here, to run once:
  //Configure baudrate
  Serial.begin(115200);


  //Configure pinout for the motor
  Motor.attach(5);

  //Start with offset
  leftOffset = analogRead(A0);
  rightOffset = analogRead(A15);

  //Initial position of motor
  Motor.write(ESQUERDA);

}

void loop() {
  // put your main code here, to run repeatedly:

  switch (machineState) {
    case CALIBRATION_LEFT_LDR:
      getLdrValues();
      if (leftLdr > 300) {
        if (calibrationDebounce) {
          calibrationDebounce--;
        } else {
          machineState = CALIBRATION_RIGHT_LDR;
          leftLdrMaxValue = leftLdr - leftOffset;
          rightLdrMinValue = rightLdr - rightOffset;
          calibrationDebounce = CALIBRATION_DEBOUNCE_CYCLE;
          Motor.write(DIREITA);
        }
      } else {
        calibrationDebounce = CALIBRATION_DEBOUNCE_CYCLE;
      }
      if (printserial) {
        printserial--;
      } else {
        printserial = 500;
        Serial.print("Esquerda: ");
        Serial.println(leftLdr);
      }

      break;
    case CALIBRATION_RIGHT_LDR:
      getLdrValues();
      if (rightLdr > 300) {
        if (calibrationDebounce) {
          calibrationDebounce--;
        } else {
          machineState = NORMAL_OPERATION;
          leftLdrMinValue = leftLdr - leftOffset;
          rightLdrMaxValue = rightLdr - rightOffset;
          Motor.write(CENTRO);
        }
      } else {
        calibrationDebounce = CALIBRATION_DEBOUNCE_CYCLE;
      }
      Serial.print("Direita: ");
      Serial.println(rightLdr);
      break;
    case NORMAL_OPERATION:
      loopingControl();
      break;


    case CHECK_CALIBRATION:
      Serial.print("Direita maximo: ");
      Serial.println(rightLdrMaxValue);
      Serial.print("Direita minimo: ");
      Serial.println(rightLdrMinValue);

      Serial.print("Esquerda maximo: ");
      Serial.println(leftLdrMaxValue);
      Serial.print("Esquerda maximo: ");
      Serial.println(leftLdrMinValue);
      break;
  }


}

void getLdrValues(void) {
  leftLdr = analogRead(A0);
  rightLdr = analogRead(A15);

}

void setLdrRealValues(void) {
  leftLdrReal = leftLdr - leftOffset;
  rightLdrReal = rightLdr - rightOffset;
}

int getMotorAngle(void) {
  static int angleMotor = angle;
  int angleRemappedLeft = 0;
  int angleRemappedRight = 0;
  int angle_limitLeft = 0;
  angleRemappedLeft = (int)((90 * leftLdrReal / (leftLdrMaxValue - leftOffset)));

  angleRemappedLeft = -1*angleRemappedLeft;
  if (angleRemappedLeft < -50){
    angleRemappedLeft = -1 * angleRemappedLeft;
  }

  angleRemappedRight = (int)((180 * rightLdrReal / (rightLdrMaxValue - rightOffset)));

  if (printserial) {
    printserial--;
  } else {
    printserial = 30;
    Serial.print(angleRemappedLeft);
    Serial.print(",");
    Serial.print(angleRemappedRight);
    Serial.print(",");
    Serial.print(leftLdrReal);
    Serial.print(",");
    Serial.println(rightLdrReal);
  }
  if (leftLdrReal > rightLdrReal && leftLdrReal > 30) {

    if (angleRemappedLeft < 0){
      angleRemappedLeft = 0;
    }else if(angleRemappedLeft > CENTRO){
      angleRemappedLeft = CENTRO;
    }
    return angleRemappedLeft;
  } else if (rightLdrReal > leftLdrReal && rightLdrReal > 30) {

    if(angleRemappedRight < CENTRO){
      angleRemappedRight = CENTRO;
    }else if(angleRemappedRight > 180){
      angleRemappedRight = 180;
    }
    return angleRemappedRight;
  } else {
    return CENTRO;
  }

}

void setMotorAngle(int angle) {
  Motor.write(angle);
}

void loopingControl(void) {

  getLdrValues();

  setLdrRealValues();

  if (!debounce_angle) {
    debounce_angle = VALOR_DEBOUNCE;
    angle = getMotorAngle();
  } else {
    debounce_angle--;
  }

  setMotorAngle(angle);


}
