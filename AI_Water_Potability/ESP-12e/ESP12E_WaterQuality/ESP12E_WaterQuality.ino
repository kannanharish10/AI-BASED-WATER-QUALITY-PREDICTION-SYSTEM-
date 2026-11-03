/*
 * ============================================================
 * ESP-12E / NodeMCU (ESP8266) - Water Quality Sensor Firmware
 * ============================================================
 * Board: "NodeMCU 1.0 (ESP-12E Module)" or "Generic ESP8266 Module"
 * Baud Rate: 115200
 *
 * Pin Connections:
 * - TDS Sensor Analog Output -> ESP-12E A0 (ADC0)
 * - TDS Sensor VCC           -> 3.3V (or 5V if NodeMCU VIN)
 * - TDS Sensor GND           -> GND
 * 
 * Data Output Format:
 * TDS:<value>,TEMP:<value>
 * ============================================================
 */

#define TDS_PIN A0
#define VREF 3.3           // ESP-12E ADC reference voltage (NodeMCU has internal voltage divider to 3.3V)
#define SCOUNT 30          // Number of sample points for median filtering

int analogBuffer[SCOUNT];
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0;

// Default temperature (25 °C) or update with a live sensor (e.g., DS18B20 on D4)
float temperature = 25.0;

int getMedianNum(int bArray[], int iFilterLen) {
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++) {
    for (i = 0; i < iFilterLen - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
  else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  return bTemp;
}

void setup() {
  Serial.begin(115200);
  pinMode(TDS_PIN, INPUT);
  delay(1000);
  Serial.println("ESP-12E Water Quality Sensor Ready");
}

void loop() {
  static unsigned long analogSampleTimepoint = millis();
  if (millis() - analogSampleTimepoint > 40U) {     // Sample analog pin every 40ms
    analogSampleTimepoint = millis();
    analogBuffer[analogBufferIndex] = analogRead(TDS_PIN);
    analogBufferIndex++;
    if (analogBufferIndex == SCOUNT)
      analogBufferIndex = 0;
  }

  static unsigned long printTimepoint = millis();
  if (millis() - printTimepoint > 2000U) {          // Send data every 2 seconds
    printTimepoint = millis();
    for (byte i = 0; i < SCOUNT; i++)
      analogBufferTemp[i] = analogBuffer[i];

    // Read median analog value and convert to voltage
    float averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float)VREF / 1024.0;

    // Temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);
    float compensationVoltage = averageVoltage / compensationCoefficient;

    // Standard Gravity TDS formula:
    float tdsValue = (133.42 * pow(compensationVoltage, 3) 
                    - 255.86 * pow(compensationVoltage, 2) 
                    + 857.39 * compensationVoltage) * 0.5;

    if (tdsValue < 0) tdsValue = 0;

    // Format expected by Python AI Water Quality Dashboard:
    Serial.print("TDS:");
    Serial.print(tdsValue, 2);
    Serial.print(",TEMP:");
    Serial.println(temperature, 2);
  }
}
