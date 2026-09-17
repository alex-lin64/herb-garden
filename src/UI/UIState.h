#pragma once

enum class Screen
{
    // "home" pages carousel
    HOME,
    LIGHTS,
    FANS,
    WATER,
    MODE,
    TEMP,

    // manual mode versions of the screens
    LIGHTS_MANUAL,
    FANS_MANUAL,
    WATER_MANUAL
};

constexpr Screen CAROUSEL_SCREENS_AUTO[] = {
    Screen::HOME,
    Screen::LIGHTS,
    Screen::FANS,
    Screen::WATER,
    Screen::MODE,
    Screen::TEMP,
};

constexpr int CAROUSEL_SCREEN_COUNT_AUTO = sizeof(CAROUSEL_SCREENS_AUTO) / sizeof(CAROUSEL_SCREENS_AUTO[0]);

constexpr Screen CAROUSEL_SCREENS_MANUAL[] = {
    Screen::HOME,
    Screen::LIGHTS_MANUAL,
    Screen::FANS_MANUAL,
    Screen::WATER_MANUAL,
    Screen::MODE,
    Screen::TEMP,
};

constexpr int CAROUSEL_SCREEN_COUNT_MANUAL = sizeof(CAROUSEL_SCREENS_MANUAL) / sizeof(CAROUSEL_SCREENS_MANUAL[0]);

struct UIState
{
    Screen screen = Screen::HOME;

    int carouselIndex = 0;

    // Used by scrolling/selectable pages
    int selectedOption = 0;

    // Used when editing a value
    int editValue = 0;

    // Used for multi-part values such as timeout
    int editField = 0;

    // Whether the current edit has been changed but not saved
    bool hasUnsavedChanges = false;
};
