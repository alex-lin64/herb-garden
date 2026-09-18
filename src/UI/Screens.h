#pragma once

#include <Arduino.h>
#include "../State/State.h"
#include "UIState.h"
#include <U8g2lib.h>

class UI;

using Display = U8G2_SH1106_128X64_NONAME_F_HW_I2C;

void drawHeader(
    Display &display,
    const SystemState &state,
    const char *title);
void drawScreenSaver(Display &display);
void drawErrorScreen(Display &display);
void drawHomeScreen(Display &display, const SystemState &state);
void drawLightsScreen(
    Display &display,
    const SystemState &state,
    const UIState &uiState);
void drawFansScreen(
    Display &display,
    const SystemState &state,
    const UIState &uiState);
void drawWaterScreen(
    Display &display,
    const SystemState &state, const UIState &uiState);
void drawModeScreen(
    Display &display,
    const SystemState &state,
    const UIState &uiState);
void drawLightsManualScreen(
    Display &display,
    const SystemState &state);
void drawFansManualScreen(
    Display &display,
    const SystemState &state);
void drawWaterManualScreen(
    Display &display,
    const SystemState &state);
