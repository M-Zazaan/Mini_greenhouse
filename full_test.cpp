#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

#define soilPin 7
#define relayPump 8
#define relayLED 9
#define ldrPin A1

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long previousMillis = 0;
const long restInterval = 120000;   // 2 minutes
bool pumpCycleDone = false;

void setup() {
  pinMode(soilPin, INPUT);
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
  int ldrValue = analogRead(ldrPin);

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // 🌙 DARK DETECTION
  if (ldrValue < 500)   // adjust if needed
    digitalWrite(relayLED, LOW);   // LED ON
  else
    digitalWrite(relayLED, HIGH);  // LED OFF

  // 💧 SOIL DRY → RUN PUMP
  if (soilState == 1 && !pumpCycleDone) {

    digitalWrite(relayPump, LOW);  // Pump ON
    delay(7000);
    digitalWrite(relayPump, HIGH); // Pump OFF

    previousMillis = millis();
    pumpCycleDone = true;
  }

  // wait 2 minutes before next cycle
  if (pumpCycleDone && millis() - previousMillis >= restInterval) {
    pumpCycleDone = false;
  }

  // 🖥 LCD DISPLAY
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
