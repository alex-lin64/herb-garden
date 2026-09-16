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

    void markScreenDirty() { screenDirty = true; }  // Mark the screen as dirty to force a redraw
    void markScreenClean() { screenDirty = false; } // Mark the screen as clean after a redraw

private:
    Hardware *hardware = nullptr;
    UIState uiState;

    bool screenDirty = true; // Flag to indicate if the screen needs to be redrawn

    void handleInput();
    void updateDisplay(SystemState &state);
};