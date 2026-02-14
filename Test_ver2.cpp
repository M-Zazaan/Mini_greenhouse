#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD address (usually 0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

int ldrPin = 7;        // LDR digital output
int tempPin = A0;      // temperature sensor analog output
int relayLight = 8;    // relay channel 1
int relayFan = 9;      // relay channel 2

void setup()
{
  pinMode(ldrPin, INPUT);
  pinMode(relayLight, OUTPUT);
  pinMode(relayFan, OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("System Starting");
  delay(2000);
  lcd.clear();
}

void loop()
{
  int lightStatus = digitalRead(ldrPin);
  int tempValue = analogRead(tempPin);

  // Convert LM35 reading to Celsius
  float voltage = tempValue * (5.0 / 1023.0);
  float temperature = voltage * 100;

  // ===== LCD DISPLAY =====
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print((char)223); // degree symbol
  lcd.print("C ");

  lcd.setCursor(0,1);

  if(lightStatus == HIGH)
  {
    lcd.print("Light: BRIGHT ");
    digitalWrite(relayLight, LOW);   // relay OFF
  }
  else
  {
    lcd.print("Light: DARK   ");
    digitalWrite(relayLight, HIGH);  // relay ON
  }

  // Fan control by temperature
  if(temperature > 30)
  {
    digitalWrite(relayFan, HIGH);   // fan ON
  }
  else
  {
    digitalWrite(relayFan, LOW);    // fan OFF
  }

  delay(1000);
}
