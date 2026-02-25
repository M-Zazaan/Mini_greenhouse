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
const long restInterval = 120000; // 2 minutes
bool pumpResting = false;
bool pumpRunning = false;

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

  int soilState = digitalRead(soilPin);  // 1 = Dry
  int lightState = digitalRead(ldrPin);  // 0 = Dark (usually)

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

 // ===== LIGHT CONTROL (FIXED LOGIC) =====
bool isDark = false;

if (lightState == HIGH) {   // HIGH = Dark (your module behavior)
  digitalWrite(relayLED, LOW);   // LED ON
  isDark = true;
} else {                    // LOW = Bright
  digitalWrite(relayLED, HIGH);  // LED OFF
  isDark = false;
}

  // ===== PUMP CONTROL =====
  if (soilState == 1 && !pumpResting) {

    pumpRunning = true;
    digitalWrite(relayPump, LOW);  // Pump ON
    delay(7000);
    digitalWrite(relayPump, HIGH); // Pump OFF
    pumpRunning = false;

    pumpResting = true;
    previousMillis = millis();
  }

  if (pumpResting && millis() - previousMillis >= restInterval) {
    pumpResting = false;
  }

  if (soilState == 0) {
    digitalWrite(relayPump, HIGH);
    pumpResting = false;
    pumpRunning = false;
  }

  // ===== LCD DISPLAY =====
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(temp);
  lcd.print("C H:");
  lcd.print(hum);
  lcd.print("%");

  lcd.setCursor(0,1);

  if (soilState == 1)
    lcd.print("Dry ");
  else
    lcd.print("Wet ");

  if (pumpRunning)
    lcd.print("PumpON ");
  else
    lcd.print("PumpOFF ");

  if (isDark)
    lcd.print("Dark");
  else
    lcd.print("Bright");

  delay(1000);
}
