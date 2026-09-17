#include "Navigation.h"
#include "../../src/State/State.h"

bool processInput(UIState &uiState, SystemState &systemState, InputEvent input)
{
    switch (uiState.screen)
    {
    case Screen::HOME:
        return handleHome(uiState, systemState, input);
    case Screen::LIGHTS:
    case Screen::FANS:
    case Screen::WATER:
    case Screen::MODE:
    case Screen::TEMP:
    case Screen::LIGHTS_MANUAL:
    case Screen::FANS_MANUAL:
    case Screen::WATER_MANUAL:
        return false;
    }

    return false; // No change in screen
}

bool handleHome(UIState &uiState, SystemState &systemState, InputEvent input)
{
    const Screen *carouselScreens;
    int carouselScreenCount;

    // Determine which carousel to use based on the system mode
    if (systemState.settings.modeAuto)
    {
        carouselScreens = CAROUSEL_SCREENS_AUTO;
        carouselScreenCount = CAROUSEL_SCREEN_COUNT_AUTO;
    }
    else
    {
        carouselScreens = CAROUSEL_SCREENS_MANUAL;
        carouselScreenCount = CAROUSEL_SCREEN_COUNT_MANUAL;
    }

    if (input == InputEvent::ROTATE_CW)
    {
        uiState.carouselIndex = (uiState.carouselIndex + 1) % carouselScreenCount;
        uiState.screen = carouselScreens[uiState.carouselIndex];
        return true; // Indicate that the screen has changed
    }
    if (input == InputEvent::ROTATE_CCW)
    {
        uiState.carouselIndex = (uiState.carouselIndex - 1 + carouselScreenCount) % carouselScreenCount;
        uiState.screen = carouselScreens[uiState.carouselIndex];
        return true; // Indicate that the screen has changed
    }
    if (input == InputEvent::BACK || input == InputEvent::ROTARY_PUSH || input == InputEvent::CONFIRM)
    {
        // TODO -- nothing mapped to these inputs on the home screen yet
        return false;
    }
    return false; // No change in screen
}
