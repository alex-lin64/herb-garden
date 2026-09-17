#include <Arduino.h>

#include "Navigation.h"
#include "../../src/State/State.h"

namespace
{
    constexpr int HOURS_PER_DAY = 24;
    constexpr int MINUTES_PER_HOUR = 60;
    constexpr int LIGHT_FREQUENCY_MIN = 1;
    constexpr int LIGHT_FREQUENCY_MAX = 9;

    constexpr int START_TIME_OPTION = 0;
    constexpr int FREQUENCY_OPTION = 2;
    constexpr int TIME_OPTION_COUNT = 2;
    constexpr int HOUR_FIELD = 0;

    int wrapValue(int value, int delta, int minimum, int maximum)
    {
        int range = maximum - minimum + 1;
        int normalizedValue = value - minimum + delta;

        normalizedValue = (normalizedValue % range + range) % range;

        return normalizedValue + minimum;
    }

    void adjustLightSchedule(
        LightSchedule &schedule,
        int selectedOption,
        int editField,
        int delta)
    {
        if (selectedOption == FREQUENCY_OPTION)
        {
            schedule.frequencyDays = wrapValue(
                schedule.frequencyDays,
                delta,
                LIGHT_FREQUENCY_MIN,
                LIGHT_FREQUENCY_MAX);
            return;
        }

        int &hour = selectedOption == START_TIME_OPTION
                        ? schedule.startHour
                        : schedule.endHour;

        int &minute = selectedOption == START_TIME_OPTION
                          ? schedule.startMinute
                          : schedule.endMinute;

        if (editField == HOUR_FIELD)
        {
            hour = wrapValue(hour, delta, 0, HOURS_PER_DAY - 1);
        }
        else
        {
            minute = wrapValue(minute, delta, 0, MINUTES_PER_HOUR - 1);
        }
    }
}

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
        adjustLightSchedule(
            uiState.editSchedule,
            uiState.selectedOption,
            uiState.editField,
            1);
        return true;
    }

    if (input == InputEvent::ROTATE_CCW)
    {
        adjustLightSchedule(
            uiState.editSchedule,
            uiState.selectedOption,
            uiState.editField,
            -1);
        return true;
    }

    if (input == InputEvent::ROTARY_PUSH ||
        input == InputEvent::CONFIRM)
    {
        if (uiState.selectedOption < TIME_OPTION_COUNT &&
            uiState.editField == HOUR_FIELD)
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