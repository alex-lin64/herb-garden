#pragma once

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

    // Outputs
    bool lightsOn = false;
    bool fansOn = false;
    bool watering = false;
};