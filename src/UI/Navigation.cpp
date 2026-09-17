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
        return handleLights(uiState, systemState, input);
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

bool handleLights(
    UIState &uiState,
    SystemState &systemState,
    InputEvent input)
{
    switch (uiState.mode)
    {
    case UIMode::VIEW:
        return handleLightsView(uiState, systemState, input);

    case UIMode::SELECT:
        return handleLightsSelect(uiState, systemState, input);

    case UIMode::EDIT:
        return handleLightsEdit(uiState, systemState, input);
    }

    return false;
}

bool handleLightsView(
    UIState &uiState,
    SystemState &systemState,
    InputEvent input)
{
    Carousel activeCarousel = getActiveCarousel(systemState);

    if (input == InputEvent::ROTATE_CW)
    {
        uiState.carouselIndex =
            (uiState.carouselIndex + 1) % activeCarousel.count;

        uiState.screen =
            activeCarousel.screens[uiState.carouselIndex];

        return true;
    }

    if (input == InputEvent::ROTATE_CCW)
    {
        uiState.carouselIndex =
            (uiState.carouselIndex - 1 + activeCarousel.count) % activeCarousel.count;

        uiState.screen =
            activeCarousel.screens[uiState.carouselIndex];

        return true;
    }

    if (input == InputEvent::ROTARY_PUSH ||
        input == InputEvent::CONFIRM)
    {
        uiState.mode = UIMode::SELECT;
        uiState.selectedOption = 0;

        return true;
    }

    if (input == InputEvent::BACK)
    {
        uiState.screen = Screen::HOME;
        uiState.mode = UIMode::VIEW;
        uiState.carouselIndex = 0;

        return true;
    }

    return false;
}

bool handleLightsSelect(
    UIState &uiState,
    SystemState &systemState,
    InputEvent input)
{
    if (input == InputEvent::ROTATE_CW)
    {
        uiState.selectedOption++;
        return true;
    }

    if (input == InputEvent::ROTATE_CCW)
    {
        uiState.selectedOption--;
        return true;
    }

    if (input == InputEvent::ROTARY_PUSH ||
        input == InputEvent::CONFIRM)
    {
        uiState.mode = UIMode::EDIT;
        uiState.editField = 0;

        return true;
    }

    if (input == InputEvent::BACK)
    {
        uiState.mode = UIMode::VIEW;

        return true;
    }

    return false;
}

bool handleLightsEdit(
    UIState &uiState,
    SystemState &systemState,
    InputEvent input)
{
    if (input == InputEvent::ROTATE_CW)
    {
        // TODO: Increase current field
        return true;
    }

    if (input == InputEvent::ROTATE_CCW)
    {
        // TODO: Decrease current field
        return true;
    }

    if (input == InputEvent::ROTARY_PUSH ||
        input == InputEvent::CONFIRM)
    {
        // TODO: Move to next field
        return true;
    }

    if (input == InputEvent::BACK)
    {
        uiState.mode = UIMode::SELECT;

        return true;
    }

    return false;
}