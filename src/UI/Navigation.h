#pragma once

#include "UIState.h"
#include "../Hardware/Input.h"
#include "../State/State.h"

void processInput(UIState &uiState, SystemState &systemState, InputEvent input);

void handleCarouselNavigation(
    UIState &uiState,
    InputEvent input);

void handleEditPageNavigation(
    UIState &uiState,
    SystemState &systemState,
    InputEvent input);