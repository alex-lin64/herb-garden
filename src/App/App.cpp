#include "App.h"
#include "Config.h"
#include <time.h>

void App::begin()
{
    Serial.begin(9600);

    hardware.begin();
    scheduleController.begin(state);
    ui.begin(hardware);
}

void App::update()
{
    unsigned long now = millis();

    // Keep Wi-Fi/NTP state updated. NTP supplies the clock used by schedules.
    hardware.updateWiFiNTP();

    // Refresh schedule anchors and automatic outputs once per second.
    if (state.settings.modeAuto &&
        now - lastClockUpdate >= PERIPHERAL_INTERVAL)
    {
        lastClockUpdate = now;
        if (getLocalTime(&localTime, 0))
        {
            scheduleController.update(state, mktime(&localTime));
        }
    }

    // Resolve manual outputs immediately, without waiting for the clock.
    if (!state.settings.modeAuto)
    {
        state.lightsOn = state.settings.manualLightsOn;
        state.fansOn = state.settings.manualFansOn;
        state.watering = state.settings.manualWaterOn;
    }

    // Only touch each GPIO when its desired output changes.
    if (state.lightsOn != appliedLightsOn)
    {
        hardware.setLights(state.lightsOn);
        appliedLightsOn = state.lightsOn;
    }
    if (state.fansOn != appliedFansOn)
    {
        hardware.setFans(state.fansOn);
        appliedFansOn = state.fansOn;
    }
    if (state.watering != appliedWatering)
    {
        hardware.setWater(state.watering);
        appliedWatering = state.watering;
    }

    // Read sensors periodically instead of on every loop iteration.
    if (now - lastSensorUpdate >= SENSOR_INTERVAL)
    {
        lastSensorUpdate = now;

        if (hardware.updateSensors(state.sensorReadings))
        {
            ui.markScreenDirty();
        }
    }

    // Process user input and redraw the display when needed.
    ui.update(state);
}
