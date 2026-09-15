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

class App
{
public:
    void begin();
    void update();

private:
    void initWiFi();

    void initWebServer();

    void handleRoot();
    void updateSensors();
    void updateDisplay();
    void showSensorError();

    Adafruit_SHT31 sht31;
    WebServer server;

    SystemState state;
    Hardware hardware;

    unsigned long lastSensorUpdate = 0;
    unsigned long lastDisplayUpdate = 0;

    unsigned long SENSOR_INTERVAL = 1000;
    unsigned long DISPLAY_INTERVAL = 1000;
};