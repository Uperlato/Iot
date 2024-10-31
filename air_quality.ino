#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>

#define MQ_135 A0

#define DHTPIN D1
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

float m = -0.3376; //Slope
float b = 0.7165; //Y-Intercept
float R0 = 6.31; //Sensor Resistance in fresh air from previous code

WiFiClient client;

long myChannelNumber = 2695806; // Channel id
const char myWriteAPIKey[] = "ZHCO0IJIWZNZCJPL";

void setup() {
  Serial.begin(9600);
  pinMode(MQ_135, INPUT);

  WiFi.begin("Lili", "13120302");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("NodeMCU is connected!");
  Serial.println(WiFi.localIP());

  dht.begin();

  ThingSpeak.begin(client);
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
  }

  float sensor_volt; //Define variable for sensor voltage
  float RS_gas; //Define variable for sensor resistance
  float ratio; //Define variable for ratio
  int sensorValue;//Variable to store the analog values from MQ-135
  float ppm_log; //Get ppm value in linear scale according to the the ratio value
  float ppm; //Convert ppm value to log scale

  sensorValue = analogRead(MQ_135); //Read analog values of sensor
  sensor_volt = sensorValue*(5.0/1023.0); //Convert analog values to voltage
  RS_gas = ((5.0*1.0)/sensor_volt)-1.0; //Get value of RS in a gas
  ratio = RS_gas/R0; // Get ratio RS_gas/RS_air
  ppm_log = (log10(ratio)-b)/m; //Get ppm value in linear scale according to the ratio value
  ppm = pow(10, ppm_log); //Convert ppm value to log scale

  Serial.println();
  Serial.println("Temperature = "+ (String) temperature);
  delay(1000);
  Serial.println("Humidity = "+ (String) humidity);
  delay(1000);
  Serial.println("Air Quality = "+ (String) sensorValue);
  delay(1000);
  Serial.println("Our desired PPM = "+ (String) ppm);
  delay(1000);

  ThingSpeak.writeField(myChannelNumber, 1, temperature, myWriteAPIKey);
  delay(5000);
  ThingSpeak.writeField(myChannelNumber, 2, humidity, myWriteAPIKey);
  delay(5000);
  ThingSpeak.writeField(myChannelNumber, 3, sensorValue, myWriteAPIKey);
  delay(5000);
  ThingSpeak.writeField(myChannelNumber, 4, ppm, myWriteAPIKey);
  delay(5000);
  
  delay(2000);
}
