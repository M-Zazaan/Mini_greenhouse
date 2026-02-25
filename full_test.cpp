#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define soilPin 7
#define relayPin 8
#define ledPin 3
#define ldrPin A1

LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long previousMillis = 0;
const long restTime = 120000;   // 2 minutes
bool pumpRunning = false;

void setup() {
  pinMode(soilPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  digitalWrite(relayPin, HIGH); // pump OFF

  lcd.init();
  lcd.backlight();
}

void loop() {

  // 🌙 LDR DARK DETECTION
  int ldrValue = analogRead(ldrPin);

  if (ldrValue < 400) {
    digitalWrite(ledPin, HIGH);   // dark
  } else {
    digitalWrite(ledPin, LOW);    // light
  }

  int soilState = digitalRead(soilPin);

  // DRY SOIL = LOW (adjust from module knob)
  if (soilState == LOW && !pumpRunning) {

    lcd.clear();
    lcd.print("Soil Dry");
    lcd.setCursor(0,1);
    lcd.print("Pump ON");

    digitalWrite(relayPin, LOW); // pump ON
    delay(7000);

    digitalWrite(relayPin, HIGH); // pump OFF
    pumpRunning = true;
    previousMillis = millis();

    lcd.clear();
    lcd.print("Resting...");
  }

  if (pumpRunning && millis() - previousMillis >= restTime) {
    pumpRunning = false;
  }

  if (soilState == HIGH) {
    lcd.clear();
    lcd.print("Soil Wet");
    digitalWrite(relayPin, HIGH);
    pumpRunning = false;
  }

  delay(500);
}
