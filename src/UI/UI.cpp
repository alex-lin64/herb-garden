#include "UI.h"
#include "Screens.h"
#include "Navigation.h"

void UI::begin(Hardware &hardware)
{
    this->hardware = &hardware;
}

void UI::update(SystemState &state)
{
    handleInput(state);

    if (screenDirty)
    {
        updateDisplay(state);
        markScreenClean();
    }
}

// Handle user input and update the UI state accordingly
void UI::handleInput(SystemState &state)
{
    InputEvent input = hardware->getInput();

    if (input == InputEvent::NONE)
        return;

    if (processInput(uiState, state, input))
    {
        markScreenDirty();
    }
}

void UI::updateDisplay(SystemState &state)
{
    auto &display = hardware->getDisplay();

    switch (uiState.screen)
    {
    case Screen::SCREEN_SAVER:
        drawScreenSaver(display);
        break;
    case Screen::HOME:
        drawHomeScreen(display, state);
        break;
    case Screen::LIGHTS:
        drawLightsScreen(display, state, uiState);
        break;
    case Screen::FANS:
        drawFansScreen(display, state, uiState);
        break;
    case Screen::WATER:
        drawWaterScreen(display, state, uiState);
        break;
    case Screen::ERROR:
        drawErrorScreen(display);
        break;
    case Screen::MODE:
        drawModeScreen(display, state, uiState);
        break;
    case Screen::LIGHTS_MANUAL:
        drawLightsManualScreen(display, state, uiState);
        break;
    case Screen::FANS_MANUAL:
        drawFansManualScreen(display, state, uiState);
        break;
    case Screen::WATER_MANUAL:
        drawWaterManualScreen(display, state);
        break;
    default:
        drawHomeScreen(display, state);
        break;
    }
}