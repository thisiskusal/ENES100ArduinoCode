unsigned long time;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("\n Time: ");
  Serial.print(millis());
  delay(500);
}
