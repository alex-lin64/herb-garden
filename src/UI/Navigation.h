#pragma once

#include "UIState.h"
#include "../Hardware/Input.h"
#include "../State/State.h"

struct Carousel
{
    const Screen *screens;
    int count;
};

Carousel getActiveCarousel(const SystemState &systemState);

bool processInput(UIState &uiState, SystemState &systemState, InputEvent input);

bool handleHome(
    UIState &uiState,
    SystemState &systemState,
    InputEvent input);
