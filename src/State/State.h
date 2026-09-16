#pragma once
#include "Config.h"

struct SystemSettings
{
    bool modeAuto = DEFAULT_MODE_AUTO;
    bool useFahrenheit = DEFAULT_USE_FAHRENHEIT;

    int brightness = DEFAULT_BRIGHTNESS;

    int timeoutMinutes = DEFAULT_TIMEOUT_MINUTES;
    int timeoutSeconds = DEFAULT_TIMEOUT_SECONDS;
};

struct SensorReadings
{
    float temperatureC = 0.0f;
    float temperatureF = 0.0f;
    float humidity = 0.0f;
    bool floatClosed = false;
};

struct SystemState
{
    SensorReadings sensorReadings;
    SystemSettings settings;

    // Outputs
    bool lightsOn = false;
    bool fansOn = false;
    bool watering = false;
};