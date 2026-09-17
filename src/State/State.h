#pragma once
#include "Config.h"

struct LightSchedule
{
    int startHour = DEFAULT_LIGHT_START_HOUR;
    int startMinute = DEFAULT_LIGHT_START_MINUTE;
    int endHour = DEFAULT_LIGHT_END_HOUR;
    int endMinute = DEFAULT_LIGHT_END_MINUTE;
    int frequencyDays = DEFAULT_LIGHT_FREQUENCY_DAYS;
};

constexpr int LIGHT_OPTION_COUNT = 3;

struct SystemSettings
{
    bool modeAuto = DEFAULT_MODE_AUTO;
    bool useFahrenheit = DEFAULT_USE_FAHRENHEIT;

    int brightness = DEFAULT_BRIGHTNESS;

    int timeoutMinutes = DEFAULT_TIMEOUT_MINUTES;
    int timeoutSeconds = DEFAULT_TIMEOUT_SECONDS;

    LightSchedule lightSchedule;
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