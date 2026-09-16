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
    BRIGHTNESS,
    TIMEOUT,

    // auto mode "home" pages edit mode
    LIGHTS_AUTO_EDIT,
    FANS_AUTO_EDIT,
    WATER_AUTO_EDIT,

    // manual mode "home"
    LIGHTS_MAN_EDIT,
    FANS_MAN_EDIT,
    WATER_MAN_EDIT
};

constexpr Screen CAROUSEL_SCREENS[] = {
    Screen::HOME,
    Screen::LIGHTS,
    Screen::FANS,
    Screen::WATER,
    Screen::MODE,
    Screen::TEMP,
    Screen::BRIGHTNESS,
    Screen::TIMEOUT,
};

constexpr int CAROUSEL_SCREEN_COUNT = sizeof(CAROUSEL_SCREENS) / sizeof(CAROUSEL_SCREENS[0]);

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
