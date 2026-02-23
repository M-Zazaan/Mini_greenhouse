#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// ----- DHT -----
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ----- LCD -----
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ----- PINS -----
const int relay1 = 8;   // Pump (IN1)
const int relay2 = 9;   // Not used
const int ldrPin = 7;   // LDR DO

// ----- SETTINGS -----
float tempThreshold = 23.0;
unsigned long pumpDuration = 5000;      // 5 seconds
unsigned long pumpInterval = 300000;    // 5 minutes

unsigned long lastPumpTime = 0;
unsigned long pumpStartTime = 0;
bool pumpRunning = false;

void setup() {

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(ldrPin, INPUT);

  digitalWrite(relay1, HIGH); // OFF (active LOW)
  digitalWrite(relay2, HIGH); // OFF

  lcd.init();
  lcd.backlight();

  dht.begin();
}

void loop() {

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // -------- SENSOR ERROR CHECK --------
  if (isnan(temperature) || isnan(humidity)) {
    lcd.setCursor(0,0);
    lcd.print("Sensor Error   ");
    lcd.setCursor(0,1);
    lcd.print("Check DHT      ");
    return;
  }

  unsigned long currentMillis = millis();

  // -------- PUMP LOGIC --------
  if (temperature >= tempThreshold) {

    if (!pumpRunning && (currentMillis - lastPumpTime >= pumpInterval)) {
      digitalWrite(relay1, LOW);   // Pump ON
      pumpRunning = true;
      pumpStartTime = currentMillis;
      lastPumpTime = currentMillis;
    }

    if (pumpRunning && (currentMillis - pumpStartTime >= pumpDuration)) {
      digitalWrite(relay1, HIGH);  // Pump OFF
      pumpRunning = false;
    }

  } else {
    digitalWrite(relay1, HIGH);  // OFF
    pumpRunning = false;
  }

  // -------- LDR DIGITAL READ --------
  int lightState = digitalRead(ldrPin);

  // -------- LCD DISPLAY --------
  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(temperature);
  lcd.print("C H:");
  lcd.print(humidity);
  lcd.print("% ");

  lcd.setCursor(0,1);

  if (lightState == LOW) {
    lcd.print("Light: DARK    ");
  } else {
    lcd.print("Light: BRIGHT  ");
  }

  delay(500);
}
