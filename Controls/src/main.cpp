#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_BMP280.h>

// Sensor objects
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);  // BNO055 default address
Adafruit_BMP280 bmp;                              // BMP280 default 0x76

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("Starting I2C...");
    Wire.begin();              // REQUIRED on Teensy
    Wire.setClock(400000);     // Teensy handles 400 kHz easily

    Serial.println("Initializing BNO055...");
    if (!bno.begin()) {
        Serial.println("❌ BNO055 not detected");
        while (1) delay(10);
    }
    bno.setExtCrystalUse(true);
    Serial.println("BNO055 OK");

    Serial.println("Initializing BMP280...");
    if (!bmp.begin(0x77)) {    // Change to 0x77 if needed
        Serial.println("❌ BMP280 not detected");
        while (1) delay(10);
    }
    Serial.println("BMP280 OK");

    // Configure BMP280 sampling
    bmp.setSampling(
        Adafruit_BMP280::MODE_NORMAL,
        Adafruit_BMP280::SAMPLING_X2,    // temperature
        Adafruit_BMP280::SAMPLING_X16,   // pressure
        Adafruit_BMP280::FILTER_X16,
        Adafruit_BMP280::STANDBY_MS_500
    );

    Serial.println("Setup complete.\n");
}

void loop() {
    // --- BNO055 Orientation ---
    sensors_event_t orientationData;
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);

    float heading = orientationData.orientation.x;
    float pitch   = orientationData.orientation.y;
    float roll    = orientationData.orientation.z;

    // --- BMP280 ---
    float temperature = bmp.readTemperature();
    float pressure    = bmp.readPressure() / 100.0f; // hPa
    float altitude    = bmp.readAltitude(1013.25);   // sea-level pressure

    // --- Print ---
    Serial.println("---- Sensor Readings ----");
    Serial.printf("Heading: %.2f deg\n", heading);
    Serial.printf("Pitch:   %.2f deg\n", pitch);
    Serial.printf("Roll:    %.2f deg\n", roll);

    Serial.printf("Temp:    %.2f °C\n", temperature);
    Serial.printf("Pressure:%.2f hPa\n", pressure);
    Serial.printf("Altitude:%.2f m\n", altitude);
    Serial.println("--------------------------\n");

    delay(200);
}