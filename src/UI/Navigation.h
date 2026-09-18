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

bool handleScreenSaver(
    UIState &uiState);

bool handleError(
    UIState &uiState,
    InputEvent input);

bool handleHome(
    UIState &uiState,
    SystemState &systemState,
    InputEvent input);

bool handleLights(
    UIState &uiState,
    SystemState &systemState,
    InputEvent input);

bool handleView(
    UIState &uiState,
    SystemState &systemState,
    InputEvent input);

bool handleLightsSelect(
    UIState &uiState,
    SystemState &systemState,
    InputEvent input);

bool handleLightsEdit(
    UIState &uiState,
    SystemState &systemState,
    InputEvent input);

bool handleFans(
    UIState &uiState,
    SystemState &systemState,
    InputEvent input);

bool handleFansSelect(
    UIState &uiState,
    SystemState &systemState,
    InputEvent input);

bool handleWater(
    UIState &uiState,
    SystemState &systemState,
    InputEvent input);

bool handleWaterSelect(
    UIState &uiState,
    SystemState &systemState,
    InputEvent input);

bool handleDurationEdit(
    UIState &uiState,
    DurationSchedule &schedule,
    InputEvent input);