#include <DHT.h>

#define LDR_PIN 7
#define DHT_PIN 2
#define DHT_TYPE DHT11   // Change to DHT22 if needed

DHT dht(DHT_PIN, DHT_TYPE);

void setup()
{
  Serial.begin(9600);
  pinMode(LDR_PIN, INPUT);
  dht.begin();

  Serial.println("System Started");
}

void loop()
{
  // -------- LDR --------
  int lightValue = digitalRead(LDR_PIN);

  if(lightValue == LOW)
  {
    Serial.println("Bright Light");
  }
  else
  {
    Serial.println("Dark");
  }

  // -------- Temperature & Humidity --------
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity))
  {
    Serial.println("DHT Sensor Error!");
  }
  else
  {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  }

  Serial.println("----------------------");

  delay(2000);
}
