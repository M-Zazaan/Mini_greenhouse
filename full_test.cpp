#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define soilPin 7
#define relayPump 8
#define relayLight 9
#define ldrPin A1

LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long previousMillis = 0;
const long restTime = 120000; // 2 minutes
bool pumpResting = false;

void setup() {
  pinMode(soilPin, INPUT);
  pinMode(relayPump, OUTPUT);
  pinMode(relayLight, OUTPUT);

  digitalWrite(relayPump, HIGH);
  digitalWrite(relayLight, HIGH);

  lcd.init();
  lcd.backlight();
  dht.begin();
}

void loop() {

  // 🌡 Read DHT11
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // 🌱 Soil Check
  int soilState = digitalRead(soilPin);

  // 🌙 LDR Check
  int ldrValue = analogRead(ldrPin);

  // ===== LCD DISPLAY =====
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(temperature);
  lcd.print("C H:");
  lcd.print(humidity);

  lcd.setCursor(0,1);
  if (soilState == LOW)
    lcd.print("Soil: DRY ");
  else
    lcd.print("Soil: WET ");

  // ===== PUMP CONTROL (SOIL ONLY) =====
  if (soilState == LOW && !pumpResting) {

    digitalWrite(relayPump, LOW); // Pump ON
    delay(7000);
    digitalWrite(relayPump, HIGH); // Pump OFF

    pumpResting = true;
    previousMillis = millis();
  }

  if (pumpResting && millis() - previousMillis >= restTime) {
    pumpResting = false;
  }

  if (soilState == HIGH) {
    digitalWrite(relayPump, HIGH);
    pumpResting = false;
  }

  // ===== LIGHT CONTROL USING RELAY 2 =====
  if (ldrValue < 400) {
    digitalWrite(relayLight, LOW); // Light ON (dark)
  } else {
    digitalWrite(relayLight, HIGH); // Light OFF
  }

  delay(1000);
}
