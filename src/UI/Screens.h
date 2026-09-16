#pragma once

#include <Arduino.h>
#include "../State/State.h"
#include <U8g2lib.h>

class UI;

void drawScreenSaver(U8G2_SH1106_128X64_NONAME_F_HW_I2C &display);
void drawHomeScreen(U8G2_SH1106_128X64_NONAME_F_HW_I2C &display, SystemState &state);
