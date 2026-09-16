#pragma once

#include <Arduino.h>
#include <Adafruit_SHT31.h>
#include <U8g2lib.h>
#include <ESP32Encoder.h>
#include <Bounce2.h>

#include "../State/State.h"
#include "Input.h"

class Hardware
{
public:
    void begin();

    void updateSensors(
        SensorReadings &sensorReadings);

    InputEvent getInput();

    U8G2_SH1106_128X64_NONAME_F_HW_I2C &getDisplay();

private:
    void initI2C();
    void initSHT31();
    void initDisplay();
    void initPins();
    void initInputs();

    bool wasPressed(uint8_t pin, bool &lastState);

    Adafruit_SHT31 sht31;

    U8G2_SH1106_128X64_NONAME_F_HW_I2C display{
        U8G2_R0,
        U8X8_PIN_NONE};

    ESP32Encoder encoder;

    Bounce2::Button backButton;
    Bounce2::Button confirmButton;
    Bounce2::Button rotaryPushButton;

    int64_t lastEncoderCount = 0;
};