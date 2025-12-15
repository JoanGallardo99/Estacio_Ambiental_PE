#include <HTTPClient.h>
#include <WiFi.h>

#include <string>

#include "DHT.h"

// WIFI
const char* ssid = "SSID";
const char* pass = "PASS";

// SENTILO
const String baseUrl = "http://147.83.83.21:8081/data/grup_3-102@provider";
const String IDENTITY_KEY =
    "61b39a9081e1ab55fedb391e10778cadcd8cedcbf98810e91ce6aa2ce70925f9";

// SENSORS
#define DHTPIN 14
#define DHTTYPE DHT22
#define LDRPIN 34
#define LED_VERD 25
#define LED_VERMELL 26

DHT dht(DHTPIN, DHTTYPE);

const unsigned long SAMPLE_INTERVAL = 5000;
unsigned long lastSample = 0;

void setup() {
    Serial.begin(115200);
    delay(500);

    WiFi.begin(ssid, pass);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected!");

    dht.begin();

    pinMode(LED_VERD, OUTPUT);
    pinMode(LED_VERMELL, OUTPUT);

    //Estat inicial
    digitalWrite(LED_VERD, LOW);
    digitalWrite(LED_VERMELL, HIGH);
}

bool aulaOK(float temp, float hum, float luz) 
{
  bool tempOK  = (temp >= 20.0 && temp <= 25.0);
  bool humOK   = (hum  >= 30.0 && hum  <= 70.0);
  bool luzOK   = (luz  >= 40.0);

  return tempOK && humOK && luzOK;
}


void enviaASentilo(String sensor, float valor) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected");
        return;
    }

    HTTPClient http;

    String url = baseUrl + "/" + sensor + "/" + String(valor);
    http.begin(url);
    http.addHeader("IDENTITY_KEY", IDENTITY_KEY);

    int httpCode = http.PUT("");  // Sending PUT request

    Serial.printf("Sentilo (%s) → HTTP %d\n", sensor.c_str(), httpCode);

    if (httpCode == 200) {
        Serial.print("code 200 Success");
    } else {
        Serial.print("code error");
    }

    http.end();
}

void loop() {
    unsigned long now = millis();
    if (now - lastSample < SAMPLE_INTERVAL) return;
    lastSample = now;

    // --- READINGS ---
    float hum = dht.readHumidity();
    float temp = dht.readTemperature();

    if (isnan(hum) || isnan(temp)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    int rawLdr = analogRead(LDRPIN);
    float lightPercent = (1.0 - rawLdr / 4095.0) * 100.0;

    bool aula = aulaOK(temp, hum, lightPercent);

    if (aula) 
    {
        digitalWrite(LED_VERD, HIGH);
        digitalWrite(LED_VERMELL, LOW);
    } 
    else 
    {
        digitalWrite(LED_VERD, LOW);
        digitalWrite(LED_VERMELL, HIGH);
    }


    // Enviar a sentilo
    enviaASentilo("temperature", temp);
    enviaASentilo("humidity", hum);
    enviaASentilo("lighting", lightPercent);

    // Mostrar per serial
    Serial.println("==============================");
    Serial.printf("Temp: %.2f\n", temp);
    Serial.printf("Hum: %.2f\n", hum);
    Serial.printf("Llum: %.2f\n", lightPercent);
    Serial.println("==============================\n");
}
