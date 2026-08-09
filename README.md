# ESP32 Weather Station

A simple weather monitoring project built with an ESP32, combining temperature, humidity, and pressure sensing with a live OLED display.

## Components Used

- **ESP32 Dev Board**
- **DHT11** – Temperature & Humidity sensor
- **BMP180** – Barometric pressure sensor (also measures temperature and estimates altitude)
- **OLED Display (SSD1306, 0.96", I2C)** – Live readout of all sensor values

## How It Works

- DHT11 reads temperature and humidity over a single digital data line.
- BMP180 reads pressure (and temperature as a backup) over the I2C bus.
- BMP180 reads pressure (and temperature as a backup) over the I2C bus..
- Readings are also printed to the Serial Monitor for debugging.

## Wiring

| Component | Pin | Connects to ESP32 |
|---|---|---|
| DHT11 | VCC | 3V3 |
| DHT11 | DATA | GPIO 4 |
| DHT11 | GND | GND |
| BMP180 | VCC | 3V3 |
| BMP180 | GND | GND |
| BMP180 | SDA | GPIO 21 |
| BMP180 | SCL | GPIO 22 |
| OLED | VCC | 3V3 |
| OLED | GND | GND |
| OLED | SDA | GPIO 21 |
| OLED | SCL | GPIO 22 |

> Note: BMP180 and OLED share the same I2C bus (SDA/SCL) since they use different I2C addresses (BMP180: 0x77, OLED: 0x3C).

See `weather_station_wiring_diagram.png` for a visual wiring diagram.

## Required Libraries

Install these via the Arduino IDE Library Manager (`Sketch → Include Library → Manage Libraries`):

- DHT sensor library (by Adafruit)
- Adafruit Unified Sensor
- Adafruit BMP085 Library (also works for BMP180)
- Adafruit GFX Library
- Adafruit SSD1306

## Getting Started

1. Wire the components as shown in the table above (or the diagram file).
2. Open `esp32_weather_station.ino` in the Arduino IDE.
3. Install the required libraries listed above.
4. Select your ESP32 board and correct COM port.
5. Upload the sketch.
6. Open the Serial Monitor (115200 baud) to see live readings, and check the OLED display for the live readout.

## Notes

- The DHT11 has a lower accuracy than the DHT22 but is sufficient for a demonstration project like this.
- BMP180's altitude reading is an estimate based on standard sea-level pressure and is not perfectly precise.
- If the DHT11 reading shows `N/A`, double check that its DATA pin is actually connected — this was the most common issue during development.

## Project Files

- `esp32_weather_station.ino` — Main Arduino sketch
- `weather_station_wiring_diagram.png` — Wiring diagram
