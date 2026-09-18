#pragma once

#include "../State/State.h"

enum class Screen
{
    // "home" pages carousel
    HOME,
    LIGHTS,
    FANS,
    WATER,
    MODE,

    // manual mode versions of the screens
    LIGHTS_MANUAL,
    FANS_MANUAL,
    WATER_MANUAL,

    SCREEN_SAVER,
    ERROR,
};

constexpr Screen CAROUSEL_SCREENS_AUTO[] = {
    Screen::HOME,
    Screen::LIGHTS,
    Screen::FANS,
    Screen::WATER,
    Screen::MODE,
};

constexpr int CAROUSEL_SCREEN_COUNT_AUTO = sizeof(CAROUSEL_SCREENS_AUTO) / sizeof(CAROUSEL_SCREENS_AUTO[0]);

constexpr Screen CAROUSEL_SCREENS_MANUAL[] = {
    Screen::HOME,
    Screen::LIGHTS_MANUAL,
    Screen::FANS_MANUAL,
    Screen::WATER_MANUAL,
    Screen::MODE,
};

constexpr int CAROUSEL_SCREEN_COUNT_MANUAL = sizeof(CAROUSEL_SCREENS_MANUAL) / sizeof(CAROUSEL_SCREENS_MANUAL[0]);

enum class UIMode
{
    VIEW,
    SELECT,
    EDIT
};

struct UIState
{
    Screen screen = Screen::HOME;
    UIMode mode = UIMode::VIEW;

    int carouselIndex = 0;

    // Used by scrolling/selectable pages
    int selectedOption = 0;

    // Used for multi-part values such as time
    int editField = 0;

    // Used when editing a value
    int editValue = 0;

    LightSchedule editLightSchedule;
    DurationSchedule editDurationSchedule;

    Screen errorReturnScreen = Screen::HOME;
    UIMode errorReturnMode = UIMode::VIEW;
    int errorReturnSelectedOption = 0;
    int errorReturnEditField = 0;
};
