#include "UI.h"

void UI::begin(Hardware &hardware)
{
    this->hardware = &hardware;
}

void UI::update(SystemState &state)
{
    handleInput();
    updateDisplay(state);
}

void UI::handleInput()
{
    // Encoder/button logic will go here
}

void UI::updateDisplay(SystemState &state)
{
    auto &display = hardware->getDisplay();

    display.clearBuffer();

    display.setFont(u8g2_font_ncenB08_tr);
    display.drawStr(0, 12, "HERB HUB");

    char tempText[20];
    sprintf(
        tempText,
        "%.1f F",
        state.sensorReadings.temperatureF);

    display.drawStr(0, 34, tempText);

    char humidityText[20];
    sprintf(
        humidityText,
        "%.1f %%",
        state.sensorReadings.humidity);

    display.drawStr(0, 56, humidityText);

    display.sendBuffer();
}

void UI::errorDisplay(const char *message)
{
    auto &display = hardware->getDisplay();

    display.clearBuffer();

    display.setFont(u8g2_font_ncenB08_tr);

    display.drawStr(0, 20, "SHT31 ERROR");
    display.drawStr(0, 40, "Check sensor");

    display.sendBuffer();
}