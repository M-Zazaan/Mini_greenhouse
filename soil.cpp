int sensorPin = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {

  long total = 0;

  for(int i=0; i<20; i++){
    total += analogRead(sensorPin);
    delay(5);
  }

  int moisture = total / 20;

  Serial.print("Moisture: ");
  Serial.print(moisture);
  Serial.print("  ->  ");

  if(moisture > 380)
    Serial.println("LOW Moisture");
  else if(moisture > 300)
    Serial.println("MEDIUM Moisture");
  else
    Serial.println("HIGH Moisture");

  delay(1500);
}
