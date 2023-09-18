#include <Servo.h>
Servo servo;
#define SERVO 3


void setup() {
  // put your setup code here, to run once:
  servo.attach(SERVO);
  servo.write(50);

}

void loop() {
  // put your main code here, to run repeatedly:

}
