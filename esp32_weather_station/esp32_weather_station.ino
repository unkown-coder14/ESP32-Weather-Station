/*
  ESP32 Weather Station
  Components: DHT11 (Temp/Humidity) + BMP180 (Pressure) + OLED SSD1306 (Display)

  Wiring:
  DHT11   -> VCC:3V3, DATA:GPIO4, GND:GND
  BMP180  -> VCC:3V3, GND:GND, SCL:GPIO22, SDA:GPIO21  (leave "3.3" pin unconnected)
  OLED    -> VCC:3V3, GND:GND, SCL:GPIO22, SDA:GPIO21

  Required Libraries (install via Library Manager):
  - DHT sensor library (by Adafruit)
  - Adafruit Unified Sensor
  - Adafruit BMP085 Library (works for BMP180)
  - Adafruit SSD1306
  - Adafruit GFX Library
*/

#include <Wire.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------- Built-in LED Setup ----------
#define LED_PIN 2  // Most ESP32 dev boards have a blue LED on GPIO 2

// ---------- DHT11 Setup ----------
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ---------- BMP180 Setup ----------
Adafruit_BMP085 bmp;

// ---------- OLED Setup ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C  // common address, try 0x3D if this doesn't work
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Start I2C bus (SDA = GPIO21, SCL = GPIO22 are ESP32 defaults)
  Wire.begin();

  // Init DHT11
  dht.begin();

  // Init BMP180
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
    while (1) {}
  }

  // Init OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("SSD1306 allocation failed");
    while (1) {}
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Weather Station");
  display.println("Starting...");
  display.display();
  delay(1500);
}

void loop() {
  // Blink LED to indicate a new measurement is happening
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);

  // ---------- Read DHT11 ----------
  float humidity = dht.readHumidity();
  float tempDHT = dht.readTemperature();

  // ---------- Read BMP180 ----------
  float tempBMP = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0F; // convert Pa to hPa
  float altitude = bmp.readAltitude();

  // Check DHT reading
  bool dhtOk = !(isnan(humidity) || isnan(tempDHT));

  // ---------- Print to Serial Monitor ----------
  Serial.println("---------------------------");
  if (dhtOk) {
    Serial.print("DHT11 Temp: ");
    Serial.print(tempDHT);
    Serial.println(" C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  } else {
    Serial.println("DHT11 read failed!");
  }

  Serial.print("BMP180 Temp: ");
  Serial.print(tempBMP);
  Serial.println(" C");
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");
  Serial.print("Altitude: ");
  Serial.print(altitude);
  Serial.println(" m");

  // ---------- Show on OLED ----------
  display.clearDisplay();
  display.setTextSize(1);

  int labelX = 0;   // x position for labels
  int valueX = 65;  // x position for values (creates horizontal gap)

  // Line 1: Temperature
  display.setCursor(labelX, 0);
  display.print("Temp:");
  display.setCursor(valueX, 0);
  if (dhtOk) {
    display.print(tempDHT, 1);
  } else {
    display.print(tempBMP, 1);
  }
  display.println(" C");

  // Line 2: Humidity
  display.setCursor(labelX, 18);
  display.print("Humid:");
  display.setCursor(valueX, 18);
  if (dhtOk) {
    display.print(humidity, 1);
    display.println(" %");
  } else {
    display.println("N/A");
  }

  // Line 3: Pressure
  display.setCursor(labelX, 36);
  display.print("Press:");
  display.setCursor(valueX, 36);
  display.print(pressure, 1);
  display.println("hPa");

  // Line 4: Altitude
  display.setCursor(labelX, 54);
  display.print("Alt:");
  display.setCursor(valueX, 54);
  display.print(altitude, 1);
  display.println("m");

  display.display();

  delay(2000); // DHT11 needs at least ~2s between reads
}
