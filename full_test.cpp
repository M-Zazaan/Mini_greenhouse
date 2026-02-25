#define soilPin 7
#define relayPin 8

void setup() {
  pinMode(soilPin, INPUT);
  pinMode(relayPin, OUTPUT);

  digitalWrite(relayPin, HIGH); // Pump OFF initially
  Serial.begin(9600);
}

void loop() {

  int soilState = digitalRead(soilPin);

  Serial.println(soilState);

  if (soilState == LOW) {   // If DRY
    digitalWrite(relayPin, LOW);  // Pump ON
  }
  else {                    // If WET
    digitalWrite(relayPin, HIGH); // Pump OFF
  }

  delay(500);
}
