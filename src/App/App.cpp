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

    hardware.updateWiFiNTP();

    if (now - lastSensorUpdate >= SENSOR_INTERVAL)
    {
        lastSensorUpdate = now;

        if (hardware.updateSensors(state.sensorReadings))
        {
            ui.markScreenDirty();
        }
    }

    ui.update(state);
}
