/*
 * =====================================================================
 * ESP-12E / NodeMCU - 3-Sensor AI Water Quality Telemetry Firmware
 * =====================================================================
 * Sensors:
 * 1. DS18B20 Waterproof Digital Temperature Probe (D2 / GPIO 4)
 * 2. Analog TDS Sensor (VCC on D5, Signal on A0)
 * 3. Analog Turbidity Sensor (VCC on D6, Signal on A0)
 * 
 * Multiplexing via Breadboard:
 * Since ESP8266 NodeMCU has only one analog pin (A0), we power the 
 * TDS sensor via D5 and Turbidity sensor via D6 alternately.
 * When D5 is HIGH, A0 reads TDS.
 * When D6 is HIGH, A0 reads Turbidity.
 * 
 * Breadboard Wiring Diagram:
 * -------------------------------------------------------------
 * ESP-12E Pin        Breadboard Rail / Component
 * -------------------------------------------------------------
 * 3V3 (3.3V)    ---> Breadboard Positive Power Rail (+)
 * GND           ---> Breadboard Ground Rail (-)
 * 
 * [1] DS18B20 Temperature Sensor:
 *     - RED Wire (VCC)    ---> Breadboard (+) 3.3V
 *     - BLACK Wire (GND)  ---> Breadboard (-) GND
 *     - YELLOW Wire (DATA)---> NodeMCU D2 (GPIO 4)
 *     - Resistor (4.7kΩ)  ---> Placed between YELLOW and RED
 * 
 * [2] Analog TDS Sensor Module:
 *     - VCC               ---> NodeMCU D5 (GPIO 14)
 *     - GND               ---> Breadboard (-) GND
 *     - Signal (AOUT)     ---> NodeMCU A0
 * 
 * [3] Turbidity Sensor Module:
 *     - VCC               ---> NodeMCU D6 (GPIO 12)
 *     - GND               ---> Breadboard (-) GND
 *     - Signal (AOUT)     ---> NodeMCU A0
 * -------------------------------------------------------------
 * Serial Telemetry Packet Format (115200 baud):
 * TEMP:<celsius>,TDS:<ppm>,TURB:<ntu>,WATER:<1/0>
 * =====================================================================
 */

#include <OneWire.h>
#include <DallasTemperature.h>

// Pin Definitions
#ifndef D2
  #define D2 4
#endif
#ifndef D5
  #define D5 14
#endif
#ifndef D6
  #define D6 12
#endif

#define ONE_WIRE_BUS     D2   // DS18B20 Data Line
#define TDS_POWER_PIN    D5   // Powers TDS sensor during reading
#define TURB_POWER_PIN   D6   // Powers Turbidity sensor during reading
#define ANALOG_PIN       A0   // Shared ADC pin

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Filter buffer for stable analog readings
#define SAMPLE_COUNT 15

int getMedianSample() {
  int samples[SAMPLE_COUNT];
  for (int i = 0; i < SAMPLE_COUNT; i++) {
    samples[i] = analogRead(ANALOG_PIN);
    delay(5);
  }
  // Simple bubble sort for median
  for (int i = 0; i < SAMPLE_COUNT - 1; i++) {
    for (int j = i + 1; j < SAMPLE_COUNT; j++) {
      if (samples[i] > samples[j]) {
        int temp = samples[i];
        samples[i] = samples[j];
        samples[j] = temp;
      }
    }
  }
  return samples[SAMPLE_COUNT / 2];
}

void setup() {
  Serial.begin(115200);
  
  pinMode(ONE_WIRE_BUS, INPUT_PULLUP);
  pinMode(TDS_POWER_PIN, OUTPUT);
  pinMode(TURB_POWER_PIN, OUTPUT);
  
  // Keep both analog sensors unpowered initially
  digitalWrite(TDS_POWER_PIN, LOW);
  digitalWrite(TURB_POWER_PIN, LOW);

  delay(1000);

  Serial.println("\n=======================================================");
  Serial.println("  ESP-12E 3-SENSOR AI WATER QUALITY STATION");
  Serial.println("  TDS + TURBIDITY + TEMPERATURE (DS18B20)");
  Serial.println("=======================================================");

  sensors.begin();
  int devCount = sensors.getDeviceCount();
  Serial.print("DS18B20 Probes Detected: ");
  Serial.println(devCount);
  if (devCount == 0) {
    Serial.println("⚠️ Notice: DS18B20 not yet detected on D2.");
    Serial.println("   Ensure Red->3V3, Black->GND, Yellow->D2 with 4.7k pullup.");
  } else {
    Serial.println("🟢 DS18B20 probe initialized successfully.");
  }
  Serial.println("🟢 Multiplexed ADC (A0) initialized for TDS (D5) & Turbidity (D6).");
  Serial.println("🚀 Telemetry streaming started at 115200 baud...\n");
}

void loop() {
  // ============================================================
  // 1. MEASURE WATER TEMPERATURE (DS18B20)
  // ============================================================
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  bool tempValid = (tempC != DEVICE_DISCONNECTED_C && tempC >= -10.0 && tempC <= 85.0);
  float effectiveTemp = tempValid ? tempC : 25.0;

  // ============================================================
  // 2. MEASURE TDS SENSOR (POWERED VIA D5)
  // ============================================================
  digitalWrite(TURB_POWER_PIN, LOW);
  digitalWrite(TDS_POWER_PIN, HIGH);
  delay(40); // Allow sensor front-end amplifier to settle

  int rawTDS = getMedianSample();
  digitalWrite(TDS_POWER_PIN, LOW); // Turn off power to prevent electrolysis

  float voltageTDS = rawTDS * (3.3 / 1024.0);
  // Temperature compensation coefficient
  float compCoeff = 1.0 + 0.02 * (effectiveTemp - 25.0);
  float compVoltageTDS = voltageTDS / compCoeff;

  // Standard Gravity TDS curve
  float tdsPpm = (133.42 * pow(compVoltageTDS, 3) - 255.86 * pow(compVoltageTDS, 2) + 857.39 * compVoltageTDS) * 0.5;
  if (tdsPpm < 0.0 || rawTDS < 8) tdsPpm = 0.0;

  // ============================================================
  // 3. MEASURE TURBIDITY SENSOR (POWERED VIA D6)
  // ============================================================
  digitalWrite(TDS_POWER_PIN, LOW);
  digitalWrite(TURB_POWER_PIN, HIGH);
  delay(40); // Allow optical IR LED and phototransistor to stabilize

  int rawTurb = getMedianSample();
  digitalWrite(TURB_POWER_PIN, LOW); // Turn off power

  float voltageTurb = rawTurb * (3.3 / 1024.0);
  
  // Optical Turbidity Calculation:
  // In clear water, voltage is high (~2.5V - 3.2V) -> ~0 to 2 NTU
  // In turbid/cloudy water, scattered light reduces voltage -> higher NTU
  float turbNTU = 0.0;
  if (voltageTurb >= 2.6) {
    // Clear water regime
    turbNTU = max(0.5, (3.2 - voltageTurb) * 4.0);
  } else if (voltageTurb >= 1.2) {
    // Moderately to heavily turbid regime
    turbNTU = (2.6 - voltageTurb) * 25.0 + 3.0;
  } else {
    // Highly turbid / dark sample
    turbNTU = 40.0 + (1.2 - voltageTurb) * 50.0;
  }
  turbNTU = constrain(turbNTU, 0.5, 120.0);

  // ============================================================
  // 4. WATER CONTACT VERIFICATION
  // ============================================================
  int waterDetected = 1;
  // If TDS is 0 and temperature is ambient or disconnected
  if (rawTDS < 10 && rawTurb < 15 && (!tempValid || tempC > 45.0 || tempC < 5.0)) {
    waterDetected = 0;
  }

  // ============================================================
  // 5. STREAM TELEMETRY PACKET TO DASHBOARD
  // ============================================================
  Serial.print("TEMP:");
  if (tempValid) {
    Serial.print(tempC, 2);
  } else {
    Serial.print(effectiveTemp, 2);
  }
  Serial.print(",TDS:");
  Serial.print(tdsPpm, 1);
  Serial.print(",TURB:");
  Serial.print(turbNTU, 2);
  Serial.print(",WATER:");
  Serial.println(waterDetected);

  delay(2000);
}
