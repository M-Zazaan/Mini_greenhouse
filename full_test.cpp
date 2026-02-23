#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int pumpRelay = 8;

float temp;

void setup() {
  pinMode(pumpRelay, OUTPUT);
  digitalWrite(pumpRelay, HIGH);   // relay OFF

  dht.begin();
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("System Starting");
  delay(2000);
  lcd.clear();
}

void loop() {

  temp = dht.readTemperature();

  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print((char)223);
  lcd.print("C   ");

  // ---- Pump Control ----
  if(temp >= 23) {
    digitalWrite(pumpRelay, LOW);   // ON
    lcd.setCursor(0,1);
    lcd.print("Pump: ON  ");
  }
  else if(temp <= 22) {
    digitalWrite(pumpRelay, HIGH);  // OFF
    lcd.setCursor(0,1);
    lcd.print("Pump: OFF ");
  }

  delay(2000);
}
