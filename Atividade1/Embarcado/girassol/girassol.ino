#include <Servo.h>
Servo Motor;

//DEFINES
#define VALOR_DEBOUNCE 10

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

//Protótipo
void getLdrValues(void);
void setLdrRealValues(void);
int getMotorAngle(void);
void setMotorAngle(int angle);

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
  Motor.write(90);

}

void loop() {
  // put your main code here, to run repeatedly:

  getLdrValues();

  setLdrRealValues();

  if (!debounce_angle) {
    debounce_angle = VALOR_DEBOUNCE;
    angle = getMotorAngle();
  } else {
    debounce_angle--;
  }

  setMotorAngle(angle);


  if (printserial) {
    printserial--;
  } else {
    printserial = 100;
    Serial.println(angle);
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
  if (leftLdrReal > 20 && leftLdrReal > rightLdrReal) {
    if (angleMotor < 180) {
      angleMotor++;
    }
  } else if (rightLdrReal > 20 && rightLdrReal > leftLdrReal) {
    if (angle) {
      angleMotor--;
    }
  } else {
    if (angleMotor > 90) {
      angleMotor--;
    } else if ( angleMotor < 90) {
      angleMotor++;
    }

  }

  return angleMotor;



}

void setMotorAngle(int angle) {
  Motor.write(angle);
}
