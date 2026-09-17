#include <Arduino.h>

#include "Screens.h"
#include "UI.h"
#include <time.h>

void drawHeader(
    U8G2_SH1106_128X64_NONAME_F_HW_I2C &display,
    SystemState &state,
    const char *title)
{
    display.setFont(u8g2_font_6x10_tr);

    // --------------------------------------------------
    // Current time - HH:MM, 24-hour format
    // --------------------------------------------------

    struct tm timeinfo;

    if (getLocalTime(&timeinfo, 0))
    {
        char timeText[6];

        strftime(
            timeText,
            sizeof(timeText),
            "%H:%M",
            &timeinfo);

        display.drawStr(0, 9, timeText);
    }
    else
    {
        display.drawStr(0, 9, "--:--");
    }

    // --------------------------------------------------
    // Title - centered
    // --------------------------------------------------

    int titleWidth = display.getStrWidth(title);

    display.drawStr(
        (128 - titleWidth) / 2,
        9,
        title);

    // --------------------------------------------------
    // Mode - right aligned
    // --------------------------------------------------

    const char *modeText =
        state.settings.modeAuto ? "AUTO" : "MAN";

    int modeWidth = display.getStrWidth(modeText);

    display.drawStr(
        128 - modeWidth,
        9,
        modeText);

    // --------------------------------------------------
    // Header separator
    // --------------------------------------------------

    display.drawHLine(0, 12, 128);
}

void drawScreenSaver(U8G2_SH1106_128X64_NONAME_F_HW_I2C &display)
{
    display.clearBuffer();

    display.setFont(u8g2_font_helvB14_tr);

    const char *herbText = "HERB";
    const char *hubText = "HUB";

    int herbWidth = display.getStrWidth(herbText);
    int hubWidth = display.getStrWidth(hubText);

    int gap = 5;
    int boxPadding = 8;

    int boxWidth = hubWidth + boxPadding * 2;
    int boxHeight = 30;

    int totalWidth = herbWidth + gap + boxWidth;
    int startX = (128 - totalWidth) / 2;

    int boxX = startX + herbWidth + gap;
    int boxY = (64 - boxHeight) / 2;

    // HERB
    display.setDrawColor(1);
    display.drawStr(
        startX,
        boxY + 21,
        herbText);

    // HUB box
    display.drawRBox(
        boxX,
        boxY,
        boxWidth,
        boxHeight,
        5);

    // HUB text
    display.setDrawColor(0);

    display.drawStr(
        boxX + boxPadding,
        boxY + 21,
        hubText);

    display.setDrawColor(1);

    display.sendBuffer();

    delay(1000);
}

void drawHomeScreen(
    U8G2_SH1106_128X64_NONAME_F_HW_I2C &display,
    SystemState &state)
{
    display.clearBuffer();

    // --------------------------------------------------
    // Header
    // --------------------------------------------------

    drawHeader(display, state, "HERB HUB");

    // --------------------------------------------------
    // Temperature / Humidity
    // --------------------------------------------------

    display.setFont(u8g2_font_ncenB10_tr);

    char tempText[20];

    if (state.settings.useFahrenheit)
    {
        sprintf(
            tempText,
            "%.1f F",
            state.sensorReadings.temperatureF);
    }
    else
    {
        sprintf(
            tempText,
            "%.1f C",
            state.sensorReadings.temperatureC);
    }

    char humidityText[20];

    sprintf(
        humidityText,
        "%.1f %%",
        state.sensorReadings.humidity);

    // Center temperature in left half
    int tempWidth = display.getStrWidth(tempText);

    display.drawStr(
        (64 - tempWidth) / 2,
        31,
        tempText);

    // Center humidity in right half
    int humidityWidth = display.getStrWidth(humidityText);

    display.drawStr(
        64 + (64 - humidityWidth) / 2,
        31,
        humidityText);

    // Small labels
    display.setFont(u8g2_font_6x10_tr);

    const char *tempLabel = "TEMP";
    const char *humidityLabel = "RH";

    int tempLabelWidth = display.getStrWidth(tempLabel);
    int humidityLabelWidth = display.getStrWidth(humidityLabel);

    display.drawStr(
        (64 - tempLabelWidth) / 2,
        42,
        tempLabel);

    display.drawStr(
        64 + (64 - humidityLabelWidth) / 2,
        42,
        humidityLabel);

    // --------------------------------------------------
    // Water level
    // --------------------------------------------------

    const char *waterText =
        state.sensorReadings.floatClosed
            ? "WATER OK"
            : "WATER LOW";

    int waterWidth = display.getStrWidth(waterText);

    display.drawStr(
        (128 - waterWidth) / 2,
        53,
        waterText);

    // --------------------------------------------------
    // Next watering time
    // --------------------------------------------------

    if (state.settings.modeAuto)
    {
        const char *nextWaterText = "Next: --:--";

        int nextWidth = display.getStrWidth(nextWaterText);

        display.drawStr(
            (128 - nextWidth) / 2,
            63,
            nextWaterText);
    }

    display.sendBuffer();
}

void drawLightsScreen(
    U8G2_SH1106_128X64_NONAME_F_HW_I2C &display,
    SystemState &state)
{
    display.clearBuffer();

    drawHeader(display, state, "LIGHTS");

    // Lights screen content goes here...

    display.sendBuffer();
}