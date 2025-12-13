#include <HTTPClient.h>
#include <WiFi.h>

#include <string>

#include "DHT.h"

// WIFI
const char* ssid = "SSID";
const char* pass = "PASSW";

// SENTILO
const String baseUrl = "http://147.83.83.21:8081/data/grup_3-102@provider";
const String IDENTITY_KEY =
    "61b39a9081e1ab55fedb391e10778cadcd8cedcbf98810e91ce6aa2ce70925f9";

// SENSORS
#define DHTPIN 14
#define DHTTYPE DHT22
#define LDRPIN 34
#define KYPIN_DO 32

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
