#define PUMP_PIN 8   // Pump/Relay connected to D8

void setup() {
  pinMode(PUMP_PIN, OUTPUT);
}

void loop() {
  digitalWrite(PUMP_PIN, HIGH);  // Turn pump ON
  delay(5000);                   // ON for 5 seconds

  digitalWrite(PUMP_PIN, LOW);   // Turn pump OFF
  delay(5000);                   // OFF for 5 seconds
}
