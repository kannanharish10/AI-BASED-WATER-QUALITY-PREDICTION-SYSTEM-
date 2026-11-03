import pandas as pd

# Load dataset
file_path = "dataset/water_potability.csv"

df = pd.read_csv(file_path)

print("=" * 60)
print("       WATER QUALITY DATA PREPROCESSING")
print("=" * 60)

# Fill missing values
df = df.fillna(df.median(numeric_only=True))

# Remove duplicate rows
df = df.drop_duplicates()

print("\nAfter preprocessing:")
print("Dataset shape:", df.shape)

print("\nMissing values:")
print(df.isnull().sum())

print("\nDuplicate rows:")
print(df.duplicated().sum())

# Save processed dataset
output_path = "dataset/water_potability_processed.csv"
df.to_csv(output_path, index=False)

print("\nProcessed dataset saved successfully!")
print(output_path)