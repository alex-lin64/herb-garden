#pragma once

#include <Arduino.h>
#include "UIState.h"
#include "../Hardware/Hardware.h"
#include "../State/State.h"

class UI
{
public:
    void begin(Hardware &hardware);
    void update(SystemState &state);

private:
    Hardware *hardware = nullptr;
    UIState uiState;

    void handleInput();
    void updateDisplay(SystemState &state);
    void errorDisplay(const char *message);
};