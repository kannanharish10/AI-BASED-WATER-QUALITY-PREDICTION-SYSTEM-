/*
 * ============================================================
 * ESP-12E / NodeMCU - LM35 Analog Temperature Sensor
 * ============================================================
 * Board in Arduino IDE: "NodeMCU 1.0 (ESP-12E Module)"
 * Baud Rate: 115200
 *
 * Jumper Wire Connections:
 * - LM35 Pin 1 (Vs/VCC) -> NodeMCU 3V3 (or 5V/VIN)
 * - LM35 Pin 2 (Vout)   -> NodeMCU A0 (ADC0)
 * - LM35 Pin 3 (GND)    -> NodeMCU GND
 * ============================================================
 */

#define LM35_PIN A0

void setup() {
  Serial.begin(115200);
  pinMode(LM35_PIN, INPUT);
  delay(1000);
  Serial.println("ESP-12E LM35 Sensor Initialized");
}

void loop() {
  int raw = analogRead(LM35_PIN);
  // NodeMCU internal voltage divider maps 0-3.3V to 0-1023 ADC counts
  float voltage = (raw / 1024.0) * 3300.0; // in mV
  // LM35 outputs 10mV per degree Celsius
  float temperature = voltage / 10.0;

  Serial.print("TEMP:");
  Serial.println(temperature, 2);

  delay(2000);
}
