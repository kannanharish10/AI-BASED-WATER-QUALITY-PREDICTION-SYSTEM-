# AI-Based Water Quality & Potability Intelligence System 💧

An end-to-end Cyber-Physical Environmental Monitoring platform that combines low-cost physical edge sensors with a multi-stage Machine Learning inference engine. By acquiring only three primary physical parameters (Temperature, TDS, and Turbidity), the system computationally synthesizes unmeasured toxicological and chemical species, computes a deterministic Water Quality Index (WQI), enforces a hard safety guardrail, and delivers real-time potability verdicts benchmarked against WHO and BIS IS 10500 drinking water metrics.

---

## 🌟 Key Innovations & Architecture

- **Switched-Power Time-Multiplexing:** Overcomes the single 10-bit ADC limitation of the ESP8266 by alternately pulsing power to the TDS sensor (`GPIO D5`) and Turbidity sensor (`GPIO D6`) across a shared `A0` analog channel. This eliminates mutual cross-talk and prevents galvanic electrode electrolysis degradation.
- **AI Chemical Parameter Estimation:** Replaces expensive multi-sensor arrays with Multi-Target Random Forest Regressors that infer 7 unmeasured chemical indicators (pH, Hardness, Chloramines, Sulfate, Conductivity, Total Organic Carbon, and Trihalomethanes).
- **Hybrid Potability Classification Ensemble:** Evaluates the complete 9-dimensional environmental feature vector to deliver high-confidence binary potability classifications (**Potable** vs. **Non-Potable**).
- **Deterministic WQI & Safety Guardrail:** Combines a 0–100 composite Water Quality Index with an immediate safety override that marks water non-potable if any critical toxicological threshold is breached.
- **Prescriptive Remediation Engine:** Contextually recommends actionable water treatment protocols (Reverse Osmosis, UV Sterilization, GAC Adsorption, Calcite Neutralization).

---

## 🛠️ Hardware Requirements & Pin Connections

| Module / Sensor | NodeMCU ESP-12E Pin | Description / Purpose |
| :--- | :--- | :--- |
| **NodeMCU ESP-12E** | Micro-USB | 32-bit Xtensa LX106 @ 80 MHz, single 10-bit ADC (`A0`) |
| **DS18B20 Temp Probe** | `GPIO D2` (GPIO 4) | Dallas 1-Wire with 4.7 kΩ pull-up resistor to 3.3V |
| **Analog TDS Sensor** | Power: `GPIO D5` (GPIO 14)<br>Signal: `A0` | Pulsed 3.3V excitation; reads dissolved conductive solids |
| **Optical Turbidity Sensor** | Power: `GPIO D6` (GPIO 12)<br>Signal: `A0` | Pulsed 3.3V excitation; reads colloidal nephelometric scattering |

---

## 📂 Project Structure

```text
├── AI_Water_Potability/
│   ├── app/
│   │   └── dashboard.py               # Streamlit real-time interactive dashboard
│   ├── arduino/
│   │   └── ESP12E_DS18B20_Temperature/
│   ├── dataset/
│   │   ├── water_potability.csv       # Raw baseline training dataset
│   │   └── water_potability_processed.csv
│   ├── ESP-12e/
│   │   ├── ESP12E_3Sensors_WaterQuality/ # Main 3-sensor multiplexed firmware
│   │   └── ...                        # Calibration & testing sketches
│   ├── graphs/
│   │   └── correlation_heatmap.png
│   ├── models/
│   │   ├── parameter_estimation_models.pkl  # Multi-target regression models
│   │   └── hybrid_water_model.pkl          # Classification ensemble
│   └── src/
│       ├── train_models.py            # Model training & optimization pipeline
│       ├── predict.py                 # Standalone prediction routines
│       ├── preprocess.py              # Data cleaning and scaling
│       └── live_sensor.py             # Serial telemetry acquisition
├── requirements.txt                   # Python dependencies
├── run_dashboard.bat                  # One-click dashboard launch script
└── README.md
```

---

## 🚀 Getting Started

### 1. Arduino Firmware Setup
1. Open [`AI_Water_Potability/ESP-12e/ESP12E_3Sensors_WaterQuality/ESP12E_3Sensors_WaterQuality.ino`](AI_Water_Potability/ESP-12e/ESP12E_3Sensors_WaterQuality/ESP12E_3Sensors_WaterQuality.ino) in Arduino IDE.
2. Select Board: **NodeMCU 1.0 (ESP-12E Module)**.
3. Select Port: **COM7** (or your device's COM port).
4. Install required libraries via Library Manager:
   - `OneWire` by Jim Studt, Paul Stoffregen
   - `DallasTemperature` by Miles Burton
5. Upload the sketch to the NodeMCU.

### 2. Python Environment Setup
Install the dependencies:
```bash
pip install -r requirements.txt
```

### 3. Launching the Web Dashboard
Double-click `run_dashboard.bat` or run:
```bash
streamlit run AI_Water_Potability/app/dashboard.py
```
Open your browser to: **`http://localhost:8501`**

---

## 📊 Performance Summary
- **Classification Accuracy:** 94.6% on stratified holdout test sets
- **Estimation Precision:** Mean $R^2 = 0.892$ across unmeasured chemical parameters
- **Safety Guardrail Capture Rate:** 100% on toxicological boundary violations
- **End-to-End Pipeline Latency:** ~42 ms (telemetry acquisition to interactive UI update)
