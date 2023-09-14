#include <Servo.h>

#define backLight 13
#define btnFill 6
#define btnDrain 7
#define SERVO 3
#define solenoidHigh 9
#define photoRes A0 //0-1023, actual 750-1000


Servo servo;
int photoVal = 0;
int fillCnt = 0;
int drainCnt = 0;

void setup() {
  servo.attach(SERVO);
  pinMode(backLight, OUTPUT);
  pinMode(photoRes, INPUT);
  // pinMode(solenoidLow, OUTPUT);
  pinMode(btnFill, INPUT);
  pinMode(btnDrain, INPUT);

  Serial.begin(9600);
}

void loop() {
  photoVal = analogRead(photoRes);
  delay(10);
  // if (photoVal > 970) {
  //   digitalWrite(solenoidLow, HIGH);
  //   Serial.println(photoVal);
  // } else if (photoVal < 850) {
  //   digitalWrite(solenoidLow, LOW);
  // }
  servo.write(10);
  delay(1000);
  servo.write(30);
  delay(1000);

  if (!digitalRead(btnFill)) {
    Serial.print("Fill: ");
    Serial.println(fillCnt); 
    fillCnt++;
  }
  if (!digitalRead(btnDrain)) {
    Serial.print("Drain: ");
    Serial.println(drainCnt); 
    drainCnt++;
  }

}
