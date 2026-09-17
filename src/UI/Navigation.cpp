#include "Navigation.h"
#include "../../src/State/State.h"

Carousel getActiveCarousel(const SystemState &systemState)
{
    if (systemState.settings.modeAuto)
    {
        return {
            CAROUSEL_SCREENS_AUTO,
            CAROUSEL_SCREEN_COUNT_AUTO};
    }

    return {
        CAROUSEL_SCREENS_MANUAL,
        CAROUSEL_SCREEN_COUNT_MANUAL};
}

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
    case Screen::LIGHTS_MANUAL:
    case Screen::FANS_MANUAL:
    case Screen::WATER_MANUAL:
        return false;
    }

    return false; // No change in screen
}

bool handleHome(UIState &uiState, SystemState &systemState, InputEvent input)
{
    Carousel activeCarousel = getActiveCarousel(systemState);

    if (input == InputEvent::ROTATE_CW)
    {
        uiState.carouselIndex = (uiState.carouselIndex + 1) % activeCarousel.count;
        uiState.screen = activeCarousel.screens[uiState.carouselIndex];
        return true; // Indicate that the screen has changed
    }
    if (input == InputEvent::ROTATE_CCW)
    {
        uiState.carouselIndex = (uiState.carouselIndex - 1 + activeCarousel.count) % activeCarousel.count;
        uiState.screen = activeCarousel.screens[uiState.carouselIndex];
        return true; // Indicate that the screen has changed
    }
    if (input == InputEvent::BACK || input == InputEvent::ROTARY_PUSH || input == InputEvent::CONFIRM)
    {
        // TODO -- nothing mapped to these inputs on the home screen yet
        return false;
    }
    return false; // No change in screen
}
