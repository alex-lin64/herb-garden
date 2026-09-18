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

struct DurationSchedule
{
    int durationMinutes;
    int frequencyHours;
};

constexpr int DURATION_OPTION_COUNT = 2;

struct SystemSettings
{
    bool modeAuto = DEFAULT_MODE_AUTO;
    bool useFahrenheit = DEFAULT_USE_FAHRENHEIT;

    LightSchedule lightSchedule;
    DurationSchedule waterSchedule = {
        DEFAULT_WATER_DURATION_MINUTES,
        DEFAULT_WATER_FREQUENCY_HOURS};

    DurationSchedule fansSchedule = {
        DEFAULT_FANS_DURATION_MINUTES,
        DEFAULT_FANS_FREQUENCY_HOURS};
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