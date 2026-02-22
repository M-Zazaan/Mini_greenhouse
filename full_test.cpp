#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int ldrPin = 7;
int pumpRelay = 8;
int lightRelay = 9;

void setup() {
  pinMode(ldrPin, INPUT);
  pinMode(pumpRelay, OUTPUT);
  pinMode(lightRelay, OUTPUT);

  digitalWrite(pumpRelay, HIGH);
  digitalWrite(lightRelay, HIGH);

  lcd.init();
  lcd.backlight();

  dht.begin();
}

void loop() {

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int dark = digitalRead(ldrPin);

  // 🌞 LIGHT CONTROL
  if(dark == HIGH) {
    digitalWrite(lightRelay, LOW);   // LED ON
  } else {
    digitalWrite(lightRelay, HIGH);  // LED OFF
  }

  // 🌡 TEMPERATURE CONTROL
  if(temp > 28) {
    digitalWrite(pumpRelay, LOW);   // pump ON
  } else {
    digitalWrite(pumpRelay, HIGH);  // pump OFF
  }

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(temp);
  lcd.print("C H:");
  lcd.print(hum);

  lcd.setCursor(0,1);

  if(dark == HIGH)
    lcd.print("Lights: ON ");
  else
    lcd.print("Lights: OFF");

  delay(2000);
}
