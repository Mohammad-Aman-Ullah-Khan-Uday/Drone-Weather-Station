
#include "DHT.h"
#include <Wire.h>
#include <SFE_BMP180.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
SFE_BMP180 bmp180;

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing..");

  dht.begin();
  
  bool success = bmp180.begin();
  if (success) {
    Serial.println("BMP180 init success");
    Serial.println("DHT11 init success");
  } else {
    Serial.println("BMP180 init failed");
  }
}

void loop() {
  Serial.flush();
  delay(2000);

  // Reading temperature or humidity from DHT11
  float h = dht.readHumidity();
  float t = dht.readTemperature();         // Celsius
  float f = dht.readTemperature(true);     // Fahrenheit

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit
  float hi = dht.computeHeatIndex(f, h);

  char status;
  double T, P;

  // Reading temperature and pressure from BMP180
  status = bmp180.startTemperature();
  if (status != 0) {
    delay(1000);
    status = bmp180.getTemperature(T);
    
    if (status != 0) {
      status = bmp180.startPressure(3);
      
      if (status != 0) {
        delay(status);
        status = bmp180.getPressure(P, T);
        
        if (status != 0) {
          Serial.print("Pressure: ");
          Serial.print(P);
          Serial.println(" hPa");
        } else {
          Serial.println("Failed to get pressure");
        }
      } else {
        Serial.println("Failed to start pressure reading");
      }
    } else {
      Serial.println("Failed to get temperature from BMP180");
    }
  } else {
    Serial.println("Failed to start temperature reading from BMP180");
  }

  // Display results from DHT11
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %	");
  
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.println(" *F	");
  
  Serial.print("Heat index: ");
  Serial.print(hi);
  Serial.println(" *F");
}
