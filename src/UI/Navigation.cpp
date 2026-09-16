#include "Navigation.h"
#include "../../src/State/State.h"

void processInput(UIState &uiState, SystemState &systemState, InputEvent input)
{
    switch (uiState.screen)
    {
    case Screen::HOME:
    case Screen::LIGHTS:
    case Screen::FANS:
    case Screen::WATER:
    case Screen::MODE:
    case Screen::TEMP:
    case Screen::BRIGHTNESS:
    case Screen::TIMEOUT:
        handleCarouselNavigation(uiState, input);
        break;

    case Screen::LIGHTS_AUTO_EDIT:
    case Screen::FANS_AUTO_EDIT:
    case Screen::WATER_AUTO_EDIT:
    case Screen::LIGHTS_MAN_EDIT:
    case Screen::FANS_MAN_EDIT:
    case Screen::WATER_MAN_EDIT:
        handleEditPageNavigation(uiState, systemState, input);
        break;
    }
}

void handleCarouselNavigation(UIState &uiState, InputEvent input)
{
    if (input == InputEvent::ROTATE_CW)
    {
        uiState.carouselIndex = (uiState.carouselIndex + 1) % CAROUSEL_SCREEN_COUNT;
        uiState.screen = CAROUSEL_SCREENS[uiState.carouselIndex];
    }
    else if (input == InputEvent::ROTATE_CCW)
    {
        uiState.carouselIndex = (uiState.carouselIndex - 1 + CAROUSEL_SCREEN_COUNT) % CAROUSEL_SCREEN_COUNT;
        uiState.screen = CAROUSEL_SCREENS[uiState.carouselIndex];
    }
    else if (input == InputEvent::BACK)
    {
        uiState.carouselIndex = 0; // Reset to the first home screen
        uiState.screen = CAROUSEL_SCREENS[uiState.carouselIndex];
    }
    else if (input == InputEvent::ROTARY_PUSH || input == InputEvent::CONFIRM)
    {
        // TODO
    }
}

void handleEditPageNavigation(UIState &uiState, SystemState &systemState, InputEvent input)
{
    // TODO
}