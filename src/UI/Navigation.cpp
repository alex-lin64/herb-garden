#include <Arduino.h>

#include "Navigation.h"
#include "../../src/State/State.h"

namespace
{
    constexpr int HOURS_PER_DAY = 24;
    constexpr int MINUTES_PER_HOUR = 60;
    constexpr int MAX_DURATION_MINUTES = 99;
    constexpr int MAX_FREQUENCY_HOURS = 99;

    constexpr int START_TIME_OPTION = 0;
    constexpr int TIME_OPTION_COUNT = 2;
    constexpr int HOUR_FIELD = 0;
    constexpr int MINUTE_FIELD = 1;

    constexpr int DURATION_OPTION = 0;
    constexpr int AUTO_MODE_OPTION = 0;
    constexpr int MANUAL_MODE_OPTION = 1;
    constexpr int MODE_OPTION_COUNT = 2;

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

    bool isValidLightSchedule(const LightSchedule &schedule)
    {
        int startMinutes =
            schedule.startHour * MINUTES_PER_HOUR + schedule.startMinute;
        int endMinutes =
            schedule.endHour * MINUTES_PER_HOUR + schedule.endMinute;

        return endMinutes > startMinutes;
    }

    void adjustDurationSchedule(
        DurationSchedule &schedule,
        int selectedOption,
        int delta)
    {
        if (selectedOption == DURATION_OPTION)
        {
            schedule.durationMinutes = wrapValue(
                schedule.durationMinutes,
                delta,
                1,
                MAX_DURATION_MINUTES);
            return;
        }

        schedule.frequencyHours = wrapValue(
            schedule.frequencyHours,
            delta,
            1,
            MAX_FREQUENCY_HOURS);
    }

    bool handleOptionNavigation(
        UIState &uiState,
        int optionCount,
        InputEvent input)
    {
        if (input == InputEvent::ROTATE_CW)
        {
            uiState.selectedOption =
                (uiState.selectedOption + 1) % optionCount;
            return true;
        }

        if (input == InputEvent::ROTATE_CCW)
        {
            uiState.selectedOption =
                (uiState.selectedOption - 1 + optionCount) % optionCount;
            return true;
        }

        if (input == InputEvent::BACK)
        {
            uiState.mode = UIMode::VIEW;
            return true;
        }

        return false;
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
    case Screen::ERROR:
        return handleError(uiState, input);
    case Screen::HOME:
        return handleHome(uiState, systemState, input);
    case Screen::LIGHTS:
        return handleLights(uiState, systemState, input);
    case Screen::FANS:
        return handleFans(uiState, systemState, input);
    case Screen::WATER:
        return handleWater(uiState, systemState, input);
    case Screen::MODE:
        return handleModePage(uiState, systemState, input);
    case Screen::LIGHTS_MANUAL:
        return handleLightsManual(uiState, systemState, input);
    case Screen::FANS_MANUAL:
        return handleFansManual(uiState, systemState, input);
    case Screen::WATER_MANUAL:
        return handleWaterManual(uiState, systemState, input);
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

bool handleError(UIState &uiState, InputEvent input)
{
    if (input == InputEvent::NONE)
        return false;

    uiState.screen = uiState.errorReturnScreen;
    uiState.mode = uiState.errorReturnMode;
    uiState.selectedOption = uiState.errorReturnSelectedOption;
    uiState.editField = uiState.errorReturnEditField;

    return true;
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
        return handleView(uiState, systemState, input);

    case UIMode::SELECT:
        return handleLightsSelect(uiState, systemState, input);

    case UIMode::EDIT:
        return handleLightsEdit(uiState, systemState, input);
    }

    return false;
}

bool handleView(
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
    if (handleOptionNavigation(uiState, LIGHT_OPTION_COUNT, input))
        return true;

    if (input == InputEvent::ROTARY_PUSH ||
        input == InputEvent::CONFIRM)
    {
        uiState.mode = UIMode::EDIT;
        uiState.editField = HOUR_FIELD;
        uiState.editLightSchedule = systemState.settings.lightSchedule;

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
            uiState.editLightSchedule,
            uiState.selectedOption,
            uiState.editField,
            1);
        return true;
    }

    if (input == InputEvent::ROTATE_CCW)
    {
        adjustLightSchedule(
            uiState.editLightSchedule,
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
            uiState.editField = MINUTE_FIELD;
            return true;
        }

        if (!isValidLightSchedule(uiState.editLightSchedule))
        {
            // return to normal selection mode, reverting the faulty input after
            // showing error screen
            uiState.errorReturnScreen = uiState.screen;
            uiState.errorReturnMode = UIMode::SELECT;
            uiState.errorReturnSelectedOption = uiState.selectedOption;
            uiState.errorReturnEditField = HOUR_FIELD;
            uiState.screen = Screen::ERROR;
            return true;
        }

        systemState.settings.lightSchedule = uiState.editLightSchedule;
        uiState.mode = UIMode::SELECT;
        uiState.editField = HOUR_FIELD;
        return true;
    }

    if (input == InputEvent::BACK)
    {
        uiState.mode = UIMode::SELECT;
        uiState.editField = HOUR_FIELD;

        return true;
    }

    return false;
}

bool handleFans(
    UIState &uiState,
    SystemState &systemState,
    InputEvent input)
{
    switch (uiState.mode)
    {
    case UIMode::VIEW:
        return handleView(uiState, systemState, input);

    case UIMode::SELECT:
        return handleFansSelect(uiState, systemState, input);

    case UIMode::EDIT:
        return handleDurationEdit(
            uiState,
            systemState.settings.fansSchedule,
            input);
    }

    return false;
}

bool handleFansSelect(
    UIState &uiState,
    SystemState &systemState,
    InputEvent input)
{
    if (handleOptionNavigation(uiState, DURATION_OPTION_COUNT, input))
        return true;

    if (input == InputEvent::ROTARY_PUSH ||
        input == InputEvent::CONFIRM)
    {
        uiState.mode = UIMode::EDIT;
        uiState.editField = 0;
        uiState.editDurationSchedule = systemState.settings.fansSchedule;

        return true;
    }

    return false;
}

bool handleWater(
    UIState &uiState,
    SystemState &systemState,
    InputEvent input)
{
    switch (uiState.mode)
    {
    case UIMode::VIEW:
        return handleView(uiState, systemState, input);

    case UIMode::SELECT:
        return handleWaterSelect(uiState, systemState, input);

    case UIMode::EDIT:
        return handleDurationEdit(
            uiState,
            systemState.settings.waterSchedule,
            input);
    }

    return false;
}

bool handleWaterSelect(
    UIState &uiState,
    SystemState &systemState,
    InputEvent input)
{
    if (handleOptionNavigation(uiState, DURATION_OPTION_COUNT, input))
        return true;

    if (input == InputEvent::ROTARY_PUSH ||
        input == InputEvent::CONFIRM)
    {
        uiState.mode = UIMode::EDIT;
        uiState.editField = 0;
        uiState.editDurationSchedule = systemState.settings.waterSchedule;

        return true;
    }

    return false;
}

bool handleModePage(
    UIState &uiState,
    SystemState &systemState,
    InputEvent input)
{
    switch (uiState.mode)
    {
    case UIMode::VIEW:
    {
        bool changed = handleView(uiState, systemState, input);

        if (input == InputEvent::ROTARY_PUSH ||
            input == InputEvent::CONFIRM)
        {
            uiState.selectedOption =
                systemState.settings.modeAuto
                    ? AUTO_MODE_OPTION
                    : MANUAL_MODE_OPTION;
        }

        return changed;
    }

    case UIMode::SELECT:
        if (handleOptionNavigation(uiState, MODE_OPTION_COUNT, input))
            return true;

        if (input == InputEvent::ROTARY_PUSH ||
            input == InputEvent::CONFIRM)
        {
            systemState.settings.modeAuto =
                uiState.selectedOption == AUTO_MODE_OPTION;
            uiState.mode = UIMode::VIEW;
            return true;
        }

        return false;

    case UIMode::EDIT:
        return false;
    }

    return false;
}

bool handleLightsManual(
    UIState &uiState,
    SystemState &systemState,
    InputEvent input)
{
    switch (uiState.mode)
    {
    case UIMode::VIEW:
        if (input == InputEvent::ROTARY_PUSH ||
            input == InputEvent::CONFIRM)
        {
            uiState.mode = UIMode::SELECT;
            uiState.selectedOption =
                systemState.settings.manualLightsOn ? 0 : 1;
            return true;
        }

        return handleView(uiState, systemState, input);

    case UIMode::SELECT:
        if (handleOptionNavigation(uiState, 2, input))
            return true;

        if (input == InputEvent::ROTARY_PUSH ||
            input == InputEvent::CONFIRM)
        {
            systemState.settings.manualLightsOn =
                uiState.selectedOption == 0;
            uiState.mode = UIMode::VIEW;
            return true;
        }

        return false;

    case UIMode::EDIT:
        return false;
    }

    return false;
}

bool handleFansManual(
    UIState &uiState,
    SystemState &systemState,
    InputEvent input)
{
    switch (uiState.mode)
    {
    case UIMode::VIEW:
        if (input == InputEvent::ROTARY_PUSH ||
            input == InputEvent::CONFIRM)
        {
            uiState.mode = UIMode::SELECT;
            uiState.selectedOption =
                systemState.settings.manualFansOn ? 0 : 1;
            return true;
        }

        return handleView(uiState, systemState, input);

    case UIMode::SELECT:
        if (handleOptionNavigation(uiState, 2, input))
            return true;

        if (input == InputEvent::ROTARY_PUSH ||
            input == InputEvent::CONFIRM)
        {
            systemState.settings.manualFansOn =
                uiState.selectedOption == 0;
            uiState.mode = UIMode::VIEW;
            return true;
        }

        return false;

    case UIMode::EDIT:
        return false;
    }

    return false;
}

bool handleWaterManual(
    UIState &uiState,
    SystemState &systemState,
    InputEvent input)
{
    switch (uiState.mode)
    {
    case UIMode::VIEW:
        if (input == InputEvent::ROTARY_PUSH ||
            input == InputEvent::CONFIRM)
        {
            uiState.mode = UIMode::SELECT;
            uiState.selectedOption =
                systemState.settings.manualWaterOn ? 0 : 1;
            return true;
        }

        return handleView(uiState, systemState, input);

    case UIMode::SELECT:
        if (handleOptionNavigation(uiState, 2, input))
            return true;

        if (input == InputEvent::ROTARY_PUSH ||
            input == InputEvent::CONFIRM)
        {
            systemState.settings.manualWaterOn =
                uiState.selectedOption == 0;
            uiState.mode = UIMode::VIEW;
            return true;
        }

        return false;

    case UIMode::EDIT:
        return false;
    }

    return false;
}

bool handleDurationEdit(
    UIState &uiState,
    DurationSchedule &schedule,
    InputEvent input)
{
    if (input == InputEvent::ROTATE_CW)
    {
        adjustDurationSchedule(
            uiState.editDurationSchedule,
            uiState.selectedOption,
            1);
        return true;
    }

    if (input == InputEvent::ROTATE_CCW)
    {
        adjustDurationSchedule(
            uiState.editDurationSchedule,
            uiState.selectedOption,
            -1);
        return true;
    }

    if (input == InputEvent::ROTARY_PUSH ||
        input == InputEvent::CONFIRM)
    {
        schedule = uiState.editDurationSchedule;
        uiState.mode = UIMode::SELECT;
        uiState.editField = HOUR_FIELD;
        return true;
    }

    if (input == InputEvent::BACK)
    {
        uiState.mode = UIMode::SELECT;
        uiState.editField = HOUR_FIELD;
        return true;
    }

    return false;
}