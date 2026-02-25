#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

#define soilPin 7
#define ldrPin 6
#define relayPump 8
#define relayLED 9

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long previousMillis = 0;
const long restInterval = 120000;   // 2 minutes
bool pumpCycleDone = false;

void setup() {
  pinMode(soilPin, INPUT);
  pinMode(ldrPin, INPUT);
  pinMode(relayPump, OUTPUT);
  pinMode(relayLED, OUTPUT);

  digitalWrite(relayPump, HIGH);
  digitalWrite(relayLED, HIGH);

  lcd.init();
  lcd.backlight();

  dht.begin();
}

void loop() {

  int soilState = digitalRead(soilPin);
  int lightState = digitalRead(ldrPin);

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // 🌙 DARK → LED ON
  if (lightState == LOW)
    digitalWrite(relayLED, LOW);
  else
    digitalWrite(relayLED, HIGH);

  // 💧 DRY SOIL → RUN PUMP
  if (soilState == 1 && !pumpCycleDone) {

    digitalWrite(relayPump, LOW);  // Pump ON
    delay(7000);
    digitalWrite(relayPump, HIGH); // Pump OFF

    previousMillis = millis();
    pumpCycleDone = true;
  }

  if (pumpCycleDone && millis() - previousMillis >= restInterval) {
    pumpCycleDone = false;
  }

  // LCD DISPLAY
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(temp);
  lcd.print("C ");

  lcd.print("H:");
  lcd.print(hum);
  lcd.print("%");

  lcd.setCursor(0,1);

  if (soilState == 1)
    lcd.print("Soil: DRY ");
  else
    lcd.print("Soil: WET ");

  delay(2000);
}
