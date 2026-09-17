#include <Arduino.h>

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
    case Screen::SCREEN_SAVER:
        return handleScreenSaver(uiState);
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

bool handleScreenSaver(UIState &uiState)
{
    // Any input should exit the screen saver
    uiState.screen = Screen::HOME;
    uiState.carouselIndex = 0;

    return true; // Indicate that the screen has changed
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
    if (input == InputEvent::BACK)
    {
        uiState.screen = Screen::SCREEN_SAVER;
        return true; // Indicate that the screen has changed
    }
    if (input == InputEvent::ROTARY_PUSH || input == InputEvent::CONFIRM)
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
        uiState.selectedOption =
            (uiState.selectedOption + 1) % LIGHT_OPTION_COUNT;
        return true;
    }

    if (input == InputEvent::ROTATE_CCW)
    {
        uiState.selectedOption =
            (uiState.selectedOption - 1 + LIGHT_OPTION_COUNT) % LIGHT_OPTION_COUNT;
        return true;
    }

    if (input == InputEvent::ROTARY_PUSH ||
        input == InputEvent::CONFIRM)
    {
        uiState.mode = UIMode::EDIT;
        uiState.editField = 0;
        uiState.editSchedule = systemState.settings.lightSchedule;

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
        if (uiState.selectedOption == 2)
        {
            uiState.editSchedule.frequencyDays =
                uiState.editSchedule.frequencyDays % 9 + 1;
        }
        else if (uiState.selectedOption == 0)
        {
            if (uiState.editField == 0)
                uiState.editSchedule.startHour =
                    (uiState.editSchedule.startHour + 1) % 24;
            else
                uiState.editSchedule.startMinute =
                    (uiState.editSchedule.startMinute + 1) % 60;
        }
        else
        {
            if (uiState.editField == 0)
                uiState.editSchedule.endHour =
                    (uiState.editSchedule.endHour + 1) % 24;
            else
                uiState.editSchedule.endMinute =
                    (uiState.editSchedule.endMinute + 1) % 60;
        }

        return true;
    }

    if (input == InputEvent::ROTATE_CCW)
    {
        if (uiState.selectedOption == 2)
        {
            uiState.editSchedule.frequencyDays =
                (uiState.editSchedule.frequencyDays + 7) % 9 + 1;
        }
        else if (uiState.selectedOption == 0)
        {
            if (uiState.editField == 0)
                uiState.editSchedule.startHour =
                    (uiState.editSchedule.startHour + 23) % 24;
            else
                uiState.editSchedule.startMinute =
                    (uiState.editSchedule.startMinute + 59) % 60;
        }
        else
        {
            if (uiState.editField == 0)
                uiState.editSchedule.endHour =
                    (uiState.editSchedule.endHour + 23) % 24;
            else
                uiState.editSchedule.endMinute =
                    (uiState.editSchedule.endMinute + 59) % 60;
        }

        return true;
    }

    if (input == InputEvent::ROTARY_PUSH ||
        input == InputEvent::CONFIRM)
    {
        if (uiState.selectedOption < 2 && uiState.editField == 0)
        {
            uiState.editField = 1;
            return true;
        }

        systemState.settings.lightSchedule = uiState.editSchedule;
        uiState.mode = UIMode::SELECT;
        uiState.editField = 0;
        return true;
    }

    if (input == InputEvent::BACK)
    {
        uiState.mode = UIMode::SELECT;
        uiState.editField = 0;

        return true;
    }

    return false;
}