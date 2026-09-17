#pragma once

#include "UIState.h"
#include "../Hardware/Input.h"
#include "../State/State.h"

bool processInput(UIState &uiState, SystemState &systemState, InputEvent input);

bool handleHome(
    UIState &uiState,
    SystemState &systemState,
    InputEvent input);

