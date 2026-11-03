import pandas as pd
import joblib

print("=" * 60)
print("             AI WATER QUALITY SYSTEM")
print("=" * 60)

# Load trained model
model = joblib.load("models/best_water_model.pkl")

# ---------------------------------------
# SIMULATED SENSOR INPUT
# Later Arduino will provide these values
# ---------------------------------------

tds = float(input("Enter TDS (ppm): "))
temperature = float(input("Enter Temperature (°C): "))

# ---------------------------------------
# ESTIMATED PARAMETERS
# These will later be replaced/improved
# using the ML estimation system
# ---------------------------------------

ph = 7.1
turbidity = 1.8

hardness = 197.22
chloramines = 7.0
sulfate = 333.0
conductivity = 425.0
organic_carbon = 14.0
trihalomethanes = 66.0

# ---------------------------------------
# DISPLAY INPUT DATA
# ---------------------------------------

print("\n" + "-" * 40)
print("WATER PARAMETERS")
print("-" * 40)

print(f"TDS             : {tds:.2f} ppm")
print(f"Temperature     : {temperature:.2f} °C")
print(f"Estimated pH    : {ph:.2f}")
print(f"Estimated Turbidity : {turbidity:.2f} NTU")

print(f"Estimated Hardness        : {hardness:.2f}")
print(f"Estimated Chloramines     : {chloramines:.2f}")
print(f"Estimated Sulfate         : {sulfate:.2f}")
print(f"Estimated Conductivity    : {conductivity:.2f}")
print(f"Estimated Organic Carbon  : {organic_carbon:.2f}")
print(f"Estimated Trihalomethanes : {trihalomethanes:.2f}")

# ---------------------------------------
# CREATE INPUT FOR MODEL
# ---------------------------------------

input_data = pd.DataFrame([{
    "ph": ph,
    "Hardness": hardness,
    "TDS_proxy": tds,
    "Chloramines": chloramines,
    "Sulfate": sulfate,
    "Conductivity": conductivity,
    "Organic_carbon": organic_carbon,
    "Trihalomethanes": trihalomethanes,
    "Turbidity": turbidity
}])

# ---------------------------------------
# ML PREDICTION
# ---------------------------------------

prediction = model.predict(input_data)[0]

print("\n" + "=" * 40)
print("          AI PREDICTION")
print("=" * 40)

if prediction == 1:
    print("Water Quality : GOOD")
    print("Status        : POTABLE*")
else:
    print("Water Quality : POOR")
    print("Status        : NOT POTABLE*")

print("=" * 40)
print("* AI prediction only. Laboratory testing is required")
print("  for actual drinking-water safety.")