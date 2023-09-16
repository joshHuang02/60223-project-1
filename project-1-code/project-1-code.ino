#include <Servo.h>

// pins
#define backLight 13
#define pumpFill 9
#define pumpDrain 10
#define btnFill 6
#define btnDrain 7
#define SERVO 3
#define photoRes A0 //0-1023, actual 720- 980 with 5V power supplies

//consts
const bool debug = false;

const float photoHigh = 1000;
const float photoLow = 690;
const float waterHigh = 30;
const float waterLow = 0;

Servo servo;
int photoVal = 0;
float currentVol = 0;
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
  pinMode(pumpFill, OUTPUT);
  pinMode(pumpDrain, OUTPUT);

  Serial.begin(9600);
  delay(500); // wait for servo to reset
}

void loop() {
  delay(100);
  photoVal = analogRead(photoRes);

  // calcualate target volume by mapping light value to some volume
  float targetVol = mapfloat(photoVal, photoLow, photoHigh, waterLow, waterHigh);

  // calcualte the difference between current volume and target volume
  float volDiff = targetVol - currentVol;

  if (abs(volDiff) < 2.5) { // volumes are close enough, no pumping
    // do nothing
    if (debug) {
      Serial.print(photoVal);
      Serial.print(" stop at ");
      Serial.println(targetVol);
    }
    servo.write(50);
    
  } else if (volDiff > 0) { // current below target, fill
    // fill
    if (debug) {
      Serial.print(photoVal);
      Serial.print(" fill to ");
      Serial.println(targetVol);
    }

    servo.write(65);
    
  } else if (volDiff < 0) { // current above target, drain
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

  // set which state the pump should be in based on buttons 
  if (digitalRead(btnFill)) {
    state = 1; // fill
  } else if (digitalRead(btnDrain)) {
    state = 2; // drain
  } else {
    state = 0; // do nothing
  }

  // based on which state the pump is in, get total time pumped in each state
  switch (state) {
    case 1: // fill
      // TODO make pump fill
      digitalWrite(pumpFill, HIGH);
      digitalWrite(pumpDrain, LOW);
      fillTime += millis() - lastLoopTime; // essentially add up time each time loop() happens
      break;
    case 2: //drain
      // TODO make pump drain
      digitalWrite(pumpFill, LOW);
      digitalWrite(pumpDrain, HIGH);
      drainTime += millis() - lastLoopTime;
      break;
    default:
      digitalWrite(pumpFill, LOW);
      digitalWrite(pumpDrain, LOW);
      break; // nothing happens
  }

  // calculate current volume as sum of filled and drained volume, assume begin at 0
  currentVol = (float)(fillTime - drainTime) / 1000;

  if (debug || true) {
    Serial.print(targetVol);
    Serial.print(" : ");
    Serial.println(currentVol);
  }

  // record the time when this loop ends so we can get duration between this loop and last loop
  lastLoopTime = millis();
}

// same map function except this one returns floats
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}