#include <Servo.h>
Servo Motor;
int leftLdr = 0;
int rightLdr = 0;
int angle_mapped = 0;
int angle = 0;

int leftOffset = 0;
int rightOffset = 0;

int leftLdrReal = 0;
int rightLdrReal = 0;
int printserial = 100;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Motor.attach(5);

  leftOffset = analogRead(A0);
  rightOffset = analogRead(A15);

  Motor.write(0);

}

void loop() {
  // put your main code here, to run repeatedly:
  leftLdr = analogRead(A0);
  rightLdr = analogRead(A15);

  leftLdrReal = leftLdr - leftOffset;
  rightLdrReal = rightLdr - rightOffset;

  if (leftLdrReal > 100 && leftLdrReal > rightLdrReal) {
    if (angle < 180) {
      angle++;
    }
  } else if (rightLdrReal > 100 && rightLdrReal > leftLdrReal) {
    if (angle) {
      angle--;
    }
  }
  angle_mapped = angle;
  Motor.write(angle_mapped);

  if (printserial) {
    printserial--;
  } else {
    printserial = 100;
    Serial.print("Left: ");
    Serial.println(leftLdrReal);
    Serial.print("Right: ");
    Serial.println(rightLdrReal);
    Serial.print("Angle: ");
    Serial.println(angle_mapped);
  }

  delay(5);
}
