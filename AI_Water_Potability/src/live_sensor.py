import random
from datetime import datetime


def get_sensor_reading():

    tds = random.uniform(150, 300)
    temperature = random.uniform(24, 30)

    return {
        "tds": round(tds, 2),
        "temperature": round(temperature, 2),
        "time": datetime.now().strftime("%H:%M:%S")
    }