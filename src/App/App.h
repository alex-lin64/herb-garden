#pragma once

#include <Arduino.h>
#include <Adafruit_SHT31.h>
#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_GFX.h>
#include <U8g2lib.h>
#include "State/State.h"
#include "Hardware/Hardware.h"
#include "UI/UI.h"
#include "Control/ScheduleController.h"

class App
{
public:
    void begin();
    void update();

private:
    SystemState state;
    Hardware hardware;
    UI ui;
    ScheduleController scheduleController;

    unsigned long lastSensorUpdate = 0;
    unsigned long lastClockUpdate = 0;
    bool appliedLightsOn = false;
    bool appliedFansOn = false;
    bool appliedWatering = false;
    struct tm localTime = {};
};