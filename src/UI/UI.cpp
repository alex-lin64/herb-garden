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
    case Screen::HOME:
        drawHomeScreen(display, state);
        break;
    case Screen::LIGHTS:
        drawLightsScreen(display, state);
        break;
    default:
        drawHomeScreen(display, state);
        break;
    }
}