#include "App.h"
#include "Config.h"

void App::begin()
{
    Serial.begin(9600);

    hardware.begin();
    ui.begin(hardware);
}

void App::update()
{
    unsigned long now = millis();

    if (now - lastSensorUpdate >= SENSOR_INTERVAL)
    {
        lastSensorUpdate = now;
        hardware.updateSensors(state.sensorReadings);
    }

    if (now - lastDisplayUpdate >= DISPLAY_INTERVAL)
    {
        lastDisplayUpdate = now;
        ui.update(state);
    }
}
