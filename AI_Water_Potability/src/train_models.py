import pandas as pd
import joblib

from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.pipeline import Pipeline

from sklearn.linear_model import LogisticRegression
from sklearn.tree import DecisionTreeClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.neighbors import KNeighborsClassifier

from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score


# ============================================================
# 1. LOAD PROCESSED DATASET
# ============================================================

file_path = "dataset/water_potability_processed.csv"
df = pd.read_csv(file_path)

print("=" * 60)
print("           AI WATER QUALITY ML TRAINING")
print("=" * 60)

print("\nDataset shape:", df.shape)


# ============================================================
# 2. INPUTS AND TARGET
# ============================================================

X = df.drop("Potability", axis=1)
y = df["Potability"]


# ============================================================
# 3. TRAIN / TEST SPLIT
# ============================================================

X_train, X_test, y_train, y_test = train_test_split(
    X,
    y,
    test_size=0.20,
    random_state=42,
    stratify=y
)

print("\nTraining samples:", len(X_train))
print("Testing samples :", len(X_test))


# ============================================================
# 4. CREATE MODELS
# ============================================================

models = {

    "Logistic Regression": Pipeline([
        ("scaler", StandardScaler()),
        ("model", LogisticRegression(max_iter=1000))
    ]),

    "Decision Tree": DecisionTreeClassifier(
        random_state=42
    ),

    "Random Forest": RandomForestClassifier(
        n_estimators=200,
        random_state=42
    ),

    "KNN": Pipeline([
        ("scaler", StandardScaler()),
        ("model", KNeighborsClassifier(n_neighbors=5))
    ])
}


# ============================================================
# 5. TRAIN AND TEST
# ============================================================

results = []

best_model = None
best_name = ""
best_accuracy = 0

for name, model in models.items():

    print("\nTraining:", name)

    model.fit(X_train, y_train)

    predictions = model.predict(X_test)

    accuracy = accuracy_score(y_test, predictions)
    precision = precision_score(y_test, predictions, zero_division=0)
    recall = recall_score(y_test, predictions, zero_division=0)
    f1 = f1_score(y_test, predictions, zero_division=0)

    results.append({
        "Model": name,
        "Accuracy": accuracy,
        "Precision": precision,
        "Recall": recall,
        "F1_Score": f1
    })

    print("Accuracy :", round(accuracy * 100, 2), "%")
    print("Precision:", round(precision * 100, 2), "%")
    print("Recall   :", round(recall * 100, 2), "%")
    print("F1 Score :", round(f1 * 100, 2), "%")

    if accuracy > best_accuracy:
        best_accuracy = accuracy
        best_model = model
        best_name = name


# ============================================================
# 6. MODEL COMPARISON
# ============================================================

results_df = pd.DataFrame(results)

print("\n" + "=" * 60)
print("                 MODEL COMPARISON")
print("=" * 60)

print(results_df.to_string(index=False))


# ============================================================
# 7. SAVE RESULTS
# ============================================================

results_df.to_csv(
    "models/model_comparison.csv",
    index=False
)


# ============================================================
# 8. SAVE BEST MODEL
# ============================================================

joblib.dump(
    best_model,
    "models/best_water_model.pkl"
)

print("\n" + "=" * 60)
print("BEST MODEL:", best_name)
print("BEST ACCURACY:", round(best_accuracy * 100, 2), "%")
print("=" * 60)

print("\nBest model saved:")
print("models/best_water_model.pkl")

print("\nTraining completed successfully!")