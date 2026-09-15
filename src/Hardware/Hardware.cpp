#include "Hardware.h"
#include "Config.h"
#include "../Helpers/Helpers.h"
#include "../State/State.h"

void Hardware::begin()
{
    initI2C();
    initPins();
    initSHT31();
    initDisplay();
}

void Hardware::initI2C()
{
    Wire.begin(SDA_PIN, SCL_PIN);
}

void Hardware::initPins()
{
    pinMode(FLOAT_PIN, INPUT_PULLUP);
}

void Hardware::initSHT31()
{
    while (!sht31.begin(SHT31_ADDRESS))
    {
        Serial.println("Connecting to SHT31...");
        delay(1000);
    }

    Serial.println("SHT31 connected!");
}

void Hardware::initDisplay()
{
    display.begin();
    display.clearBuffer();

    display.setFont(u8g2_font_ncenB08_tr);

    display.drawStr(0, 15, "HERB HUB");
    display.drawStr(0, 35, "OLED working!");
    display.drawStr(0, 55, "128 x 64");

    display.sendBuffer();

    delay(1000);
}

void Hardware::updateSensors(
    SensorReadings &sensorReadings)
{
    // Read SHT31 sensor
    sensorReadings.temperatureC = sht31.readTemperature();
    sensorReadings.temperatureF = celsiusToFahrenheit(sensorReadings.temperatureC, -1.0f);
    sensorReadings.humidity = sht31.readHumidity();

    // Read float switch
    sensorReadings.floatClosed = digitalRead(FLOAT_PIN) == LOW;
}

U8G2_SH1106_128X64_NONAME_F_HW_I2C &Hardware::getDisplay()
{
    return display;
}