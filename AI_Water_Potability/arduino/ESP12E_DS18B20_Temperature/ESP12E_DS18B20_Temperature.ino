/*
 * ============================================================
 * ESP-12E / NodeMCU - DS18B20 Waterproof Temperature Sensor
 * ============================================================
 * Board in Arduino IDE: "NodeMCU 1.0 (ESP-12E Module)"
 * Port: COM7
 * Baud Rate: 115200
 *
 * Required Libraries (Install via Arduino Library Manager):
 * 1. "OneWire" by Paul Stoffregen
 * 2. "DallasTemperature" by Miles Burton
 *
 * Wiring Connections:
 * - RED Wire (VCC)    -> NodeMCU 3V3 (3.3V)
 * - BLACK Wire (GND)  -> NodeMCU GND
 * - YELLOW Wire (DATA)-> NodeMCU D2 (GPIO 4)
 * 
 * NOTE ON RESISTOR:
 * DS18B20 requires a 4.7kΩ (or 1kΩ - 10kΩ) resistor connected 
 * between the YELLOW data wire and RED 3.3V wire.
 * (We also activate internal pull-up in this code to assist).
 * ============================================================
 */

#include <OneWire.h>
#include <DallasTemperature.h>

// Pin D2 on NodeMCU corresponds to GPIO 4
#define ONE_WIRE_BUS D2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  pinMode(ONE_WIRE_BUS, INPUT_PULLUP);
  delay(1000);

  Serial.println("\n============================================");
  Serial.println("   ESP-12E DS18B20 WATER TEMPERATURE PROBE");
  Serial.println("============================================");

  sensors.begin();
  int deviceCount = sensors.getDeviceCount();
  Serial.print("DS18B20 Sensors detected: ");
  Serial.println(deviceCount);

  if (deviceCount == 0) {
    Serial.println("⚠️ Warning: No DS18B20 found on pin D2!");
    Serial.println("👉 Check: Red -> 3V3, Black -> GND, Yellow -> D2");
    Serial.println("👉 Ensure a 4.7k pull-up resistor is between Yellow & Red");
  } else {
    Serial.println("🟢 DS18B20 Sensor ready! Streaming live readings...");
  }
}

void loop() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  // Check if sensor is disconnected or returning error code
  if (tempC == DEVICE_DISCONNECTED_C || tempC < -50.0 || tempC > 125.0) {
    Serial.println("Temp: nanC | Sensor disconnected or missing 4.7k resistor on D2");
  } else {
    // Format parsed by Python AI Water Quality Dashboard:
    Serial.print("TEMP:");
    Serial.println(tempC, 2);
  }

  delay(2000);
}
