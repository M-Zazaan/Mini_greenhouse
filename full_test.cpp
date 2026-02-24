#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define RELAY 8
#define TEMP_PIN A0
#define LDR_PIN A1

float temp;

unsigned long previousMillis = 0;
const long pumpInterval = 300000;   // 5 minutes
const int pumpRunTime = 5000;       // 5 seconds

bool pumpRunning = false;
unsigned long pumpStartTime = 0;

void setup() {
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, HIGH); // relay OFF

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("System Starting");
  delay(2000);
  lcd.clear();
}

void loop() {

  // ===== TEMPERATURE READING =====
  int value = analogRead(TEMP_PIN);
  float voltage = value * (5.0 / 1023.0);
  temp = voltage * 100;   // LM35

  // ===== LDR READING =====
  int lightValue = analogRead(LDR_PIN);

  if(lightValue < 500)  // dark condition
    lcd.backlight();
  else
    lcd.noBacklight();

  // ===== DISPLAY =====
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print((char)223);
  lcd.print("C ");

  // ===== PUMP CONTROL =====
  unsigned long currentMillis = millis();

  if(temp >= 23) {

    if(!pumpRunning && currentMillis - previousMillis >= pumpInterval) {
      digitalWrite(RELAY, LOW); // pump ON
      pumpRunning = true;
      pumpStartTime = currentMillis;

      lcd.setCursor(0,1);
      lcd.print("Pump: ON ");
    }

    if(pumpRunning && currentMillis - pumpStartTime >= pumpRunTime) {
      digitalWrite(RELAY, HIGH); // pump OFF
      pumpRunning = false;
      previousMillis = currentMillis;

      lcd.setCursor(0,1);
      lcd.print("Pump: OFF");
    }

  } else {
    digitalWrite(RELAY, HIGH); // ensure OFF
    pumpRunning = false;

    lcd.setCursor(0,1);
    lcd.print("Pump: OFF");
  }
}
