#pragma once

#include <Arduino.h>
#include <Adafruit_SHT31.h>
#include <U8g2lib.h>
#include "../State/State.h"

class Hardware
{
public:
    void begin();

    void updateSensors(
        SensorReadings &sensorReadings);

    U8G2_SH1106_128X64_NONAME_F_HW_I2C &getDisplay();

private:
    void initI2C();
    void initSHT31();
    void initDisplay();
    void initPins();

    Adafruit_SHT31 sht31;

    U8G2_SH1106_128X64_NONAME_F_HW_I2C display{
        U8G2_R0,
        U8X8_PIN_NONE};
};