#define BLYNK_PRINT Serial

//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"
//#define BLYNK_AUTH_TOKEN            "YourAuthToken"

#define BLYNK_TEMPLATE_ID "TMPL2ZIYVwkiy"
#define BLYNK_TEMPLATE_NAME "DHT"
#define BLYNK_AUTH_TOKEN "90z3SqQ_IfFSZdCmJBWyacF5jiCxijwN"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Lili";
char pass[] = "13120302";

#define DHTPIN D1
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  // Debug console
  Serial.begin(115200);
  dht.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{
  Blynk.run();
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

Serial.println("Temperature: ");
Serial.println(temperature);
Serial.println(" °C, Humidity: ");
Serial.println(humidity);
Serial.println(" %");

  Blynk.virtualWrite(V0, humidity);
  Blynk.virtualWrite(V1, temperature);

  delay(3000); // Update every 2 seconds
}