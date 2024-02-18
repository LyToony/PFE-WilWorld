#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL52o-gK-Eu"
#define BLYNK_TEMPLATE_NAME "Temp et Servo"
#define BLYNK_AUTH_TOKEN "vT-Dh28rxpH_I5mZBGSXiiQUx_psAOsI"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

BlynkTimer timer;

#define SOIL_MOISTURE_PIN 34 // Broche à laquelle le capteur d'humidité du sol est connecté
#define DHTPIN 26 //Connect Out pin to A0 in NODE MCU
#define DHTTYPE DHT11  
#define relay 25
#define drySoil 2600
#define wetSoil 3200
String soilStatus;

DHT dht(DHTPIN, DHTTYPE);

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Toto";
char pass[] = "toto12345";

BLYNK_WRITE(V4) {
  bool Relay = param.asInt();
  if (Relay == 1) {
    digitalWrite(relay, HIGH);
  } else {
    digitalWrite(relay, LOW);
  }
}
void sendSensor()
{
  int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
  float MoisturePercentage = map(soilMoistureValue, 0, 4095, 0, 100);
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (soilMoistureValue < drySoil) {
    soilStatus = "Je suis à sec, fais péter l'eau";
    // digitalWrite(relay, HIGH);
  } else if (soilMoistureValue >= drySoil && soilMoistureValue < wetSoil) {
    soilStatus = "C'est parfait !";
  } else {
    soilStatus = "Je suis trempé";
    // digitalWrite(relay, LOW);
  }
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
    Blynk.virtualWrite(V0, t);
    Blynk.virtualWrite(V1, h);
    Blynk.virtualWrite(V2, soilStatus);
    Blynk.virtualWrite(V3, MoisturePercentage);
    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.print("    Humidity : ");
    Serial.println(h);
    Serial.print("Humidité du sol: ");
    Serial.println(soilMoistureValue);
    Serial.print("Humidité du sol (%): ");
    Serial.println(MoisturePercentage);
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  dht.begin();
  timer.setInterval(1000L, sendSensor);
}
 
void loop() {
  Blynk.run();
  timer.run();
}
