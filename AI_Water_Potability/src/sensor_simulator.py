import random
import time

print("=" * 60)
print("       WATER QUALITY SENSOR SIMULATOR")
print("=" * 60)

print("\nGenerating simulated sensor readings...\n")

try:
    while True:

        tds = random.uniform(150, 300)
        temperature = random.uniform(24, 30)

        print(
            f"TDS:{tds:.2f},TEMP:{temperature:.2f}"
        )

        time.sleep(2)

except KeyboardInterrupt:

    print("\nSensor simulation stopped.")