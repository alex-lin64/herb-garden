#include "Hardware.h"
#include "Config.h"
#include "../Helpers/Helpers.h"
#include "../State/State.h"
#include <WiFi.h>
#include <UI/Screens.h>

void Hardware::begin()
{
    initI2C();
    initPins();
    initDisplay();
    initSHT31();
    initInputs();
    initWiFiNTP();
}

void Hardware::initI2C()
{
    Wire.begin(SDA_PIN, SCL_PIN);
}

void Hardware::initPins()
{
    // Init float switch pin
    pinMode(FLOAT_SWITCH_PIN, INPUT_PULLUP);

    // Init rotary encoder and button pins
    pinMode(OLED_CONFIRM_PIN, INPUT_PULLUP);
    pinMode(OLED_PUSH_PIN, INPUT_PULLUP);
    pinMode(OLED_ROTARY_CW_PIN, INPUT_PULLUP);
    pinMode(OLED_ROTARY_CCW_PIN, INPUT_PULLUP);
    pinMode(OLED_BACK_PIN, INPUT_PULLUP);
}

void Hardware::initSHT31()
{
    while (!sht31.begin(SHT31_ADDRESS))
    {
        Serial.println("Connecting to SHT31...");
        delay(1000);
    }

    Serial.println("SHT31 connected!");
}

void Hardware::initDisplay()
{
    display.begin();
    drawScreenSaver(display);
    delay(1000);
}

void Hardware::initInputs()
{
    ESP32Encoder::useInternalWeakPullResistors = puType::up;

    encoder.attachHalfQuad(
        OLED_ROTARY_CCW_PIN,
        OLED_ROTARY_CW_PIN);

    encoder.clearCount();
    lastEncoderCount = encoder.getCount();

    backButton.attach(OLED_BACK_PIN, INPUT_PULLUP);
    backButton.interval(DEBOUNCE_MS);
    backButton.setPressedState(LOW);

    confirmButton.attach(OLED_CONFIRM_PIN, INPUT_PULLUP);
    confirmButton.interval(DEBOUNCE_MS);
    confirmButton.setPressedState(LOW);

    rotaryPushButton.attach(OLED_PUSH_PIN, INPUT_PULLUP);
    rotaryPushButton.interval(DEBOUNCE_MS);
    rotaryPushButton.setPressedState(LOW);
}

void Hardware::initWiFiNTP()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.println("Starting WiFi...");
}

bool Hardware::updateSensors(
    SensorReadings &sensorReadings)
{
    // Save previous values
    float oldTemperatureC = sensorReadings.temperatureC;
    float oldTemperatureF = sensorReadings.temperatureF;
    float oldHumidity = sensorReadings.humidity;
    bool oldFloatClosed = sensorReadings.floatClosed;

    // Read SHT31 sensor
    sensorReadings.temperatureC = sht31.readTemperature();
    sensorReadings.temperatureF =
        celsiusToFahrenheit(
            sensorReadings.temperatureC,
            -1.0f);

    sensorReadings.humidity =
        sht31.readHumidity();

    // Read float switch
    sensorReadings.floatClosed =
        digitalRead(FLOAT_SWITCH_PIN) == LOW;

    // Check whether anything changed
    return round(sensorReadings.temperatureC * 10) !=
               round(oldTemperatureC * 10) ||

           round(sensorReadings.humidity * 10) !=
               round(oldHumidity * 10) ||

           sensorReadings.floatClosed != oldFloatClosed;
}

U8G2_SH1106_128X64_NONAME_F_HW_I2C &Hardware::getDisplay()
{
    return display;
}

InputEvent Hardware::getInput()
{
    // Update buttons
    backButton.update();
    confirmButton.update();
    rotaryPushButton.update();

    // Check rotary encoder
    int64_t currentCount = encoder.getCount();
    int64_t countDifference = currentCount - lastEncoderCount;
    lastEncoderCount = currentCount;

    // Half-quad produces two raw counts per detent. Keep partial movement so
    // direction changes cannot lose a count between input polls.
    pendingEncoderCounts += countDifference;

    if (pendingEncoderCounts >= 2)
    {
        pendingEncoderCounts -= 2;
        return InputEvent::ROTATE_CW;
    }

    if (pendingEncoderCounts <= -2)
    {
        pendingEncoderCounts += 2;
        return InputEvent::ROTATE_CCW;
    }

    // Check buttons
    if (backButton.pressed())
        return InputEvent::BACK;

    if (confirmButton.pressed())
        return InputEvent::CONFIRM;

    if (rotaryPushButton.pressed())
        return InputEvent::ROTARY_PUSH;

    return InputEvent::NONE;
}

void Hardware::updateWiFiNTP()
{
    static bool ntpStarted = false;

    if (WiFi.status() == WL_CONNECTED)
    {
        if (!ntpStarted)
        {
            Serial.println("WiFi connected!");

            configTzTime(
                "EST5EDT",
                "pool.ntp.org",
                "time.nist.gov");

            ntpStarted = true;

            Serial.println("NTP started.");
        }
    }
    else
    {
        ntpStarted = false;
    }
}