#define backLight 13
#define solenoidLow 8
#define solenoidHigh 9
#define photoRes A0 //0-1023, actual 750-1000

int photoVal = 0;

void setup() {
  pinMode(backLight, OUTPUT);
  pinMode(photoRes, INPUT);
  pinMode(solenoidLow, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  photoVal = analogRead(photoRes);
  delay(10);
  if (photoVal > 930) {
    digitalWrite(solenoidLow, HIGH);
    Serial.println(photoVal);
  } else {
    digitalWrite(solenoidLow, LOW);
  }

}
