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

    // Test

    InputEvent event = hardware.getInput();

    switch (event)
    {
    case InputEvent::ROTATE_CW:
        Serial.println("CW");
        break;

    case InputEvent::ROTATE_CCW:
        Serial.println("CCW");
        break;

    case InputEvent::ROTARY_PUSH:
        Serial.println("ROTARY PUSH");
        break;

    case InputEvent::BACK:
        Serial.println("BACK");
        break;

    case InputEvent::CONFIRM:
        Serial.println("CONFIRM");
        break;

    case InputEvent::NONE:
        break;
    }
}
