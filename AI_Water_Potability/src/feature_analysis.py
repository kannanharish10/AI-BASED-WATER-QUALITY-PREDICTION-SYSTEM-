import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Load processed dataset
file_path = "dataset/water_potability_processed.csv"
df = pd.read_csv(file_path)

print("=" * 60)
print("          WATER QUALITY FEATURE ANALYSIS")
print("=" * 60)

# Correlation with Potability
correlation = df.corr(numeric_only=True)["Potability"].sort_values(
    ascending=False
)

print("\nCorrelation with Potability:")
print(correlation)

# Create graphs folder if needed
import os
os.makedirs("graphs", exist_ok=True)

# Correlation heatmap
plt.figure(figsize=(10, 8))
sns.heatmap(
    df.corr(numeric_only=True),
    annot=True,
    fmt=".2f"
)

plt.title("Water Quality Parameter Correlation")
plt.tight_layout()

plt.savefig("graphs/correlation_heatmap.png")
plt.show()

print("\nGraph saved:")
print("graphs/correlation_heatmap.png")

print("\nFeature analysis completed!")