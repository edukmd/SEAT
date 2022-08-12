#include <Servo.h>
Servo Motor;

#define VALOR_DEBOUNCE 10
int leftLdr = 0;
int rightLdr = 0;
int angle_mapped = 90;
int angle = 90;

int leftOffset = 0;
int rightOffset = 0;

int leftLdrReal = 0;
int rightLdrReal = 0;
int printserial = 100;
int debounce_angle = 5;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Motor.attach(5);

  leftOffset = analogRead(A0);
  rightOffset = analogRead(A15);

  Motor.write(90);

}

void loop() {
  // put your main code here, to run repeatedly:
  leftLdr = analogRead(A0);
  rightLdr = analogRead(A15);

  leftLdrReal = leftLdr - leftOffset;
  rightLdrReal = rightLdr - rightOffset;
  if (!debounce_angle) {
    debounce_angle = VALOR_DEBOUNCE;
    if (leftLdrReal > 20 && leftLdrReal > rightLdrReal) {
      if (angle < 180) {
        angle++;
      }
    } else if (rightLdrReal > 20 && rightLdrReal > leftLdrReal) {
      if (angle) {
        angle--;
      }
    } else {
      if (angle > 90) {
        angle--;
      } else if ( angle < 90) {
        angle++;
      }

    }

  } else {
    debounce_angle--;
  }

  angle_mapped = angle;
  Motor.write(angle_mapped);

  if (printserial) {
    printserial--;
  } else {
    printserial = 100;
    Serial.println(angle_mapped);
  }

}
