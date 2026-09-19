#include "App.h"
#include "Config.h"
#include "../Control/Lights.h"
#include <time.h>

void App::begin()
{
    Serial.begin(9600);

    hardware.begin();
    ui.begin(hardware);
}

void App::update()
{
    unsigned long now = millis();

    // Keep Wi-Fi/NTP state updated. NTP supplies the clock used by schedules.
    hardware.updateWiFiNTP();

    // Resolve the desired light state from either the schedule or manual mode.
    if (state.settings.modeAuto)
    {
        // A schedule only needs minute-level precision, so avoid polling the
        // system clock on every loop iteration.
        if (now - lastClockUpdate >= 1000)
        {
            lastClockUpdate = now;
            if (getLocalTime(&localTime, 0))
            {
                state.lightsOn =
                    resolveLightsOn(state.settings, localTime);
            }
        }
    }
    else
    {
        state.lightsOn = state.settings.manualLightsOn;
    }

    // Only touch the GPIO when the desired output changes.
    if (state.lightsOn != appliedLightsOn)
    {
        hardware.setLights(state.lightsOn);
        appliedLightsOn = state.lightsOn;
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
