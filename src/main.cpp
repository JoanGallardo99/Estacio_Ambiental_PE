#include "DHT.h"


// PINES
#define DHTPIN 14
#define DHTTYPE DHT22

#define LDRPIN 34
#define KYPIN_DO 32

// MUESTREO
const unsigned long SAMPLE_INTERVAL = 5000; // ms

DHT dht(DHTPIN, DHTTYPE);
unsigned long lastSample = 0;

void setup() {
    Serial.begin(115200);
    delay(500);

    dht.begin();
    delay(2000);

    Serial.println("Modo solo terminal iniciado.");
}

void loop() {
    unsigned long now = millis();
    if (now - lastSample < SAMPLE_INTERVAL) return;
    lastSample = now;

    // --- TEMPERATURA / HUMEDAD ---
    float hum = dht.readHumidity();
    float temp = dht.readTemperature();

    if (isnan(hum) || isnan(temp)) {
        hum = dht.readHumidity();
        temp = dht.readTemperature();
    }

    // --- LUZ ---
    int rawLdr = analogRead(LDRPIN);
    float lightPercent = (1.0 - (rawLdr / 4095.0)) * 100.0;

    // --- SONIDO (RMS) ---
    int estado = digitalRead(KYPIN_DO); // 1 = hay ruido, 0 = silencio

    // --- MOSTRAR EN TERMINAL ---
    Serial.println("=====================================");
    Serial.printf("Temperatura: %.2f °C\n", temp);
    Serial.printf("Humedad: %.2f %%\n", hum);
    Serial.printf("Luz: %.2f %%\n", lightPercent);
    Serial.printf("Ruido: %d\n", estado);
    Serial.println("=====================================\n");
}
