#include <Servo.h>

// pins
#define backLight 13
#define btnFill 6
#define btnDrain 7
#define SERVO 3
#define photoRes A0 //0-1023, actual 720- 980 with 5V power supplies

//consts
const bool debug = false;

const float photoHigh = 980;
const float photoLow = 720;
const float waterHigh = 10;
const float waterLow = 0;

Servo servo;
int photoVal = 0;
int fillCnt = 0;
int drainCnt = 0;
float currentVol = 5;
int state = 0;
int prevState = 0;
long fillTime = 0;
long drainTime = 0;

long lastLoopTime = 0;

void setup() {
  servo.attach(SERVO);
  pinMode(backLight, OUTPUT);
  pinMode(photoRes, INPUT);
  pinMode(btnFill, INPUT);
  pinMode(btnDrain, INPUT);

  Serial.begin(9600);
  delay(500); // wait for servo to reset
}

void loop() {
  photoVal = analogRead(photoRes);
  delay(100);
  //plan is to map light values to volume of water and map volume of water to time needed
  float targetVol = mapfloat(photoVal, photoLow, photoHigh, waterLow, waterHigh);

  float volDiff = targetVol - currentVol;
  if (abs(volDiff) < 0.5) {
    // do nothing
    if (debug) {
      Serial.print(photoVal);
      Serial.print(" stop at ");
      Serial.println(targetVol);
    }
    servo.write(50);
    
  } else if (volDiff > 0) {
    // fill
    if (debug) {
      Serial.print(photoVal);
      Serial.print(" fill to ");
      Serial.println(targetVol);
    }

    servo.write(65);
    
  } else if (volDiff < 0) {
    // drain
    if (debug) {
      Serial.print(photoVal);
      Serial.print(" drain to ");
      Serial.println(targetVol);
    }

    servo.write(35);
  } else {
    Serial.println("Unexpected Error");
  }

  if (digitalRead(btnFill)) {
    state = 1;
  } else if (digitalRead(btnDrain)) {
    state = 2;
  } else {
    state = 0;
  }

  // if (state != prevState) resetTimer();

  // based on which state the pump is in, get total time pumped in each state
  switch (state) {
    case 1: // fill
      // make pump fill
      fillTime += millis() - lastLoopTime; // essentially add up time each time loop() happens
      break;
    case 2: //drain
      fillTime += millis() - lastLoopTime;
      break;
    default:
      break; // nothing happens
  }

  Serial.print(fillTime);
  Serial.print(" : ");
  Serial.println(drainTime);
  lastLoopTime = millis();
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}