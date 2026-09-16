#include "UI.h"
#include "Screens.h"

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

    switch (uiState.screen)
    {
    case Screen::HOME:
        drawHomeScreen(display, state);
        break;

    default:
        drawHomeScreen(display, state);
        break;
    }
}
