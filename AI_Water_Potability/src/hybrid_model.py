import pandas as pd
import numpy as np
import joblib

from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.pipeline import Pipeline
from sklearn.ensemble import RandomForestRegressor, RandomForestClassifier
from sklearn.metrics import mean_absolute_error, accuracy_score


print("=" * 60)
print("        HYBRID AI WATER QUALITY SYSTEM")
print("=" * 60)


# ============================================================
# 1. LOAD DATA
# ============================================================

df = pd.read_csv("dataset/water_potability_processed.csv")

print("\nDataset loaded:", df.shape)


# ============================================================
# 2. REAL INPUT
# ============================================================
# Our actual low-cost hardware will provide TDS.
#
# Temperature will come from DS18B20 later.
#
# Current dataset does NOT contain temperature, so we don't
# train an artificial temperature relationship.
# ============================================================

real_input = ["TDS_proxy"]


# ============================================================
# 3. PARAMETERS TO ESTIMATE
# ============================================================

estimated_parameters = [
    "ph",
    "Hardness",
    "Chloramines",
    "Sulfate",
    "Conductivity",
    "Organic_carbon",
    "Trihalomethanes",
    "Turbidity"
]


# ============================================================
# 4. TRAIN ESTIMATION MODELS
# ============================================================

X = df[real_input]

print("\nTraining estimation models...")

estimation_models = {}

for parameter in estimated_parameters:

    y = df[parameter]

    X_train, X_test, y_train, y_test = train_test_split(
        X,
        y,
        test_size=0.20,
        random_state=42
    )

    model = RandomForestRegressor(
        n_estimators=100,
        random_state=42
    )

    model.fit(X_train, y_train)

    prediction = model.predict(X_test)

    mae = mean_absolute_error(y_test, prediction)

    estimation_models[parameter] = model

    print(
        f"{parameter:20s} MAE: {mae:.3f}"
    )


# ============================================================
# 5. SAVE ESTIMATION MODELS
# ============================================================

joblib.dump(
    estimation_models,
    "models/parameter_estimation_models.pkl"
)

print("\nEstimation models saved:")
print("models/parameter_estimation_models.pkl")


# ============================================================
# 6. TRAIN FINAL POTABILITY MODEL
# ============================================================
#
# The final model uses the estimated parameters plus TDS.
# ============================================================

feature_columns = [
    "ph",
    "Hardness",
    "TDS_proxy",
    "Chloramines",
    "Sulfate",
    "Conductivity",
    "Organic_carbon",
    "Trihalomethanes",
    "Turbidity"
]

X_final = df[feature_columns]
y_final = df["Potability"]

X_train, X_test, y_train, y_test = train_test_split(
    X_final,
    y_final,
    test_size=0.20,
    random_state=42,
    stratify=y_final
)

final_model = Pipeline([
    ("scaler", StandardScaler()),
    ("model", RandomForestClassifier(
        n_estimators=200,
        random_state=42
    ))
])

final_model.fit(X_train, y_train)

final_prediction = final_model.predict(X_test)

accuracy = accuracy_score(
    y_test,
    final_prediction
)

print("\n" + "=" * 60)
print("FINAL POTABILITY MODEL")
print("=" * 60)

print(
    "Accuracy:",
    round(accuracy * 100, 2),
    "%"
)


# ============================================================
# 7. SAVE FINAL MODEL
# ============================================================

joblib.dump(
    final_model,
    "models/hybrid_water_model.pkl"
)

print("\nFinal model saved:")
print("models/hybrid_water_model.pkl")

print("\n" + "=" * 60)
print("HYBRID MODEL TRAINING COMPLETED!")
print("=" * 60)