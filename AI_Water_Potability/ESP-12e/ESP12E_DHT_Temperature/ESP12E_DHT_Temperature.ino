/*
 * ============================================================
 * ESP-12E / NodeMCU - DHT11/DHT22 Temperature Sensor Firmware
 * ============================================================
 * Board in Arduino IDE: "NodeMCU 1.0 (ESP-12E Module)"
 * Baud Rate: 115200
 *
 * Required Library in Arduino IDE:
 * "DHT sensor library" by Adafruit
 *
 * Jumper Wire Connections:
 * - DHT VCC  -> NodeMCU 3V3 (3.3V)
 * - DHT GND  -> NodeMCU GND
 * - DHT DATA -> NodeMCU D2 (GPIO 4)
 * ============================================================
 */

#include "DHT.h"

// Define pin & sensor type
#define DHTPIN D2      // Pin D2 on NodeMCU corresponds to GPIO 4
#define DHTTYPE DHT11  // Change to DHT22 if using white sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(1000);
  dht.begin();
  Serial.println("ESP-12E DHT Temperature Sensor Initialized");
}

void loop() {
  delay(2000);

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if read failed
  if (isnan(temperature)) {
    Serial.println("Temp: nanC | Error reading DHT sensor! Check D2 pin wiring and 3.3V power.");
    return;
  }

  // Format parsed by Python AI Water Quality Dashboard:
  Serial.print("TEMP:");
  Serial.print(temperature, 2);
  Serial.print(",HUM:");
  Serial.println(humidity, 1);
}
