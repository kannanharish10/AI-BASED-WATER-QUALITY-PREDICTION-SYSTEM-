import serial
import serial.tools.list_ports
import time

print("=" * 60)
print("       ESP-12E WATER QUALITY SERIAL TEST")
print("=" * 60)

def find_esp_port():
    ports = serial.tools.list_ports.comports()
    keywords = ["ch340", "ch341", "cp210", "esp", "esp8266", "nodemcu", "usb serial"]
    for p in ports:
        if "bluetooth" in (p.description or "").lower():
            continue
        desc = f"{p.description} {p.manufacturer} {p.hwid}".lower()
        if any(k in desc for k in keywords):
            return p.device
    return "COM7"

PORT = find_esp_port()
BAUD_RATE = 115200

print(f"Connecting to ESP-12E on {PORT} at {BAUD_RATE} baud...")

esp = None

try:
    esp = serial.Serial(PORT, BAUD_RATE, timeout=2)
    time.sleep(2)

    print(f"\nESP-12E connected successfully on {PORT}!")
    print("Waiting for sensor data (press Ctrl+C to stop)...\n")

    while True:
        if esp.in_waiting > 0:
            data = esp.readline().decode("utf-8", errors="ignore").strip()

            if data:
                print("Received from ESP-12E:", data)

except serial.SerialException as e:
    print(f"\nCould not connect to ESP-12E on {PORT}.")
    print("Error:", e)

except KeyboardInterrupt:
    print("\nSerial monitoring stopped.")

finally:
    if esp is not None:
        try:
            esp.close()
        except Exception:
            pass