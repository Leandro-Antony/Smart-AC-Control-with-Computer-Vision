#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  delay(1000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) ||  isnan(t)) {
    Serial.println("Fail to read DHT11");
    return;
  }

  Serial.println("Humidity: " + String(h) + "%/t");
  Serial.println("Temperature: " + String(t) + "*C");
}
