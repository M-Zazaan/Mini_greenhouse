#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 7          // DHT data pin connected to D7
#define DHTTYPE DHT11     // Change to DHT22 if needed

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pins
const int ldrPin = A1;
const int relayPin = 8;   // Pump
const int ledPin = 9;     // LED

// Settings
const float tempThreshold = 23.0;
const unsigned long pumpDuration = 5000;        // 5 sec
const unsigned long pumpInterval = 300000;      // 5 min

unsigned long lastPumpTime = 0;
unsigned long pumpStartTime = 0;
bool pumpRunning = false;

void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  digitalWrite(relayPin, HIGH);  // Relay OFF (active LOW)
  digitalWrite(ledPin, LOW);

  lcd.init();
  lcd.backlight();
  dht.begin();
}

void loop() {

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    lcd.setCursor(0,0);
    lcd.print("Sensor Error   ");
    return;
  }

  int ldrValue = analogRead(ldrPin);

  // LED control (dark = ON)
  if (ldrValue > 600) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  unsigned long currentMillis = millis();

  // Pump logic
  if (temperature >= tempThreshold) {

    if (!pumpRunning && (currentMillis - lastPumpTime >= pumpInterval)) {
      digitalWrite(relayPin, LOW);   // Pump ON
      pumpRunning = true;
      pumpStartTime = currentMillis;
      lastPumpTime = currentMillis;
    }

    if (pumpRunning && (currentMillis - pumpStartTime >= pumpDuration)) {
      digitalWrite(relayPin, HIGH);  // Pump OFF
      pumpRunning = false;
    }

  } else {
    digitalWrite(relayPin, HIGH);
    pumpRunning = false;
  }

  // LCD Display
  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(temperature);
  lcd.print("C H:");
  lcd.print(humidity);
  lcd.print("%  ");

  lcd.setCursor(0,1);
  lcd.print("Light:");
  lcd.print(ldrValue);
  lcd.print("   ");

  delay(500);
}
