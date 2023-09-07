#define backLight 13
#define photoRes A0

void setup() {
  pinMode(backLight, OUTPUT);
  pinMode(photoRes, INPUT);

  Serial.begin(9600);
}

void loop() {
  Serial.println(analogRead(photoRes));
  delay(10);

}
