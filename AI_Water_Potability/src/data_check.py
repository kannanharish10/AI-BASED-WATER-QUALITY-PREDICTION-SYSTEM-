import pandas as pd

file_path = "dataset/water_potability.csv"

df = pd.read_csv(file_path)

print("=" * 60)
print("        AI WATER POTABILITY PROJECT")
print("=" * 60)

print("\nDataset Shape:")
print(df.shape)

print("\nColumn Names:")
for column in df.columns:
    print("-", column)

print("\nFirst 5 Rows:")
print(df.head())

print("\nMissing Values:")
print(df.isnull().sum())

print("\nDuplicate Rows:")
print(df.duplicated().sum())

print("\nPotability Distribution:")
print(df["Potability"].value_counts())

print("\nStatistical Summary:")
print(df.describe())

print("\nDATA CHECK COMPLETED!")