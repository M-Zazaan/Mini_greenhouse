void setup() {
  pinMode(8, OUTPUT);
}

void loop() {
  digitalWrite(8, LOW); // ON
  delay(5000);
  digitalWrite(8, HIGH); // OFF
  delay(5000);
}
