#pragma once

enum class Screen
{
    HOME,
    LIGHTS,
    FANS,
    WATER,
    SETTINGS
};

enum class UIMode
{
    BROWSE,
    SELECT,
    EDIT
};

struct UIState
{
    Screen screen = Screen::HOME;
    UIMode mode = UIMode::BROWSE;

    int selectedItem = 0;
};