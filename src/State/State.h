#pragma once
#include <cstdint>
#include <time.h>
#include "Config.h"

struct LightSchedule
{
    int startHour = DEFAULT_LIGHT_START_HOUR;
    int startMinute = DEFAULT_LIGHT_START_MINUTE;
    int endHour = DEFAULT_LIGHT_END_HOUR;
    int endMinute = DEFAULT_LIGHT_END_MINUTE;
};

constexpr int LIGHT_OPTION_COUNT = 2;

struct DurationSchedule
{
    int durationMinutes;
    int frequencyHours;
};

constexpr int DURATION_OPTION_COUNT = 2;

struct SystemSettings
{
    bool modeAuto = DEFAULT_MODE_AUTO;
    bool manualLightsOn = false;
    bool manualFansOn = false;
    bool manualWaterOn = false;
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
    float temperatureC = DEFAULT_TEMP_HUMID;
    float temperatureF = DEFAULT_TEMP_HUMID;
    float humidity = DEFAULT_TEMP_HUMID;
    bool floatClosed = false;
};

struct SystemState
{
    SensorReadings sensorReadings;
    SystemSettings settings;

    // Outputs
    // Resolved physical output, regardless of whether auto or manual mode is active.
    bool lightsOn = false;
    bool fansOn = false;
    bool watering = false;

    // Absolute anchors for recurring duration schedules.
    int64_t fansScheduleAnchor = 0;
    int64_t waterScheduleAnchor = 0;
};