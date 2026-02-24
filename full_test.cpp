#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int pumpRelay = 8;
int lightRelay = 9;
int ldrPin = 7;

float temp;

unsigned long lastPumpTime = 0;
const unsigned long pumpInterval = 300000; // 5 minutes
const int pumpRunTime = 5000; // 5 seconds

void setup() {
  pinMode(pumpRelay, OUTPUT);
  pinMode(lightRelay, OUTPUT);
  pinMode(ldrPin, INPUT);

  digitalWrite(pumpRelay, HIGH);   // OFF
  digitalWrite(lightRelay, HIGH);  // OFF

  dht.begin();
  lcd.init();
  lcd.backlight();

  lcd.print("System Starting");
  delay(2000);
  lcd.clear();
}

void loop() {

  temp = dht.readTemperature();
  int lightState = digitalRead(ldrPin);

  // ===== LCD DISPLAY =====
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print((char)223);
  lcd.print("C   ");

  // ===== LIGHT CONTROL =====
  if(lightState == HIGH) {   // dark
    digitalWrite(lightRelay, LOW);   // light ON
    lcd.setCursor(0,1);
    lcd.print("Light: ON ");
  }
  else {
    digitalWrite(lightRelay, HIGH);  // light OFF
    lcd.setCursor(0,1);
    lcd.print("Light: OFF");
  }

  // ===== PUMP CONTROL =====
  unsigned long currentTime = millis();

  if(temp >= 23) {

    if(currentTime - lastPumpTime >= pumpInterval) {

      digitalWrite(pumpRelay, LOW);  // pump ON
      delay(pumpRunTime);
      digitalWrite(pumpRelay, HIGH); // pump OFF

      lastPumpTime = currentTime;
    }
  }

  delay(2000);
}
