int sensorPin = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {

  int total = 0;

  for(int i=0; i<10; i++){
    total += analogRead(sensorPin);
    delay(10);
  }

  int moisture = total / 10;

  Serial.print("Value: ");
  Serial.print(moisture);
  Serial.print("  ->  ");

  if(moisture > 350)
  {
    Serial.println("LOW Moisture (Dry)");
  }
  else if(moisture > 280)
  {
    Serial.println("MEDIUM Moisture");
  }
  else
  {
    Serial.println("HIGH Moisture (Wet)");
  }

  delay(1000);
}
