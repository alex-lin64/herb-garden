#include <Arduino.h>

#include "Screens.h"
#include "UI.h"
#include <time.h>

namespace
{
    constexpr int DISPLAY_WIDTH = 128;
    constexpr int DISPLAY_HEIGHT = 64;
}

void drawHeader(
    Display &display,
    const SystemState &state,
    const char *title)
{
    constexpr int HEADER_BASELINE = 9;
    constexpr int HEADER_SEPARATOR_Y = 12;

    display.setFont(u8g2_font_6x10_tr);

    // --------------------------------------------------
    // Current time - HH:MM, 24-hour format
    // --------------------------------------------------

    struct tm timeinfo;

    if (getLocalTime(&timeinfo, 0))
    {
        char timeText[6];

        strftime(
            timeText,
            sizeof(timeText),
            "%H:%M",
            &timeinfo);

        display.drawStr(0, HEADER_BASELINE, timeText);
    }
    else
    {
        display.drawStr(0, HEADER_BASELINE, "--:--");
    }

    // --------------------------------------------------
    // Title - centered
    // --------------------------------------------------

    int titleWidth = display.getStrWidth(title);
    int titleX = (DISPLAY_WIDTH - titleWidth) / 2;

    display.drawStr(
        titleX,
        HEADER_BASELINE,
        title);

    // --------------------------------------------------
    // Mode - right aligned
    // --------------------------------------------------

    const char *modeText =
        state.settings.modeAuto ? "AUTO" : "MAN";

    int modeWidth = display.getStrWidth(modeText);

    display.drawStr(
        DISPLAY_WIDTH - modeWidth,
        HEADER_BASELINE,
        modeText);

    // --------------------------------------------------
    // Header separator
    // --------------------------------------------------

    display.drawHLine(0, HEADER_SEPARATOR_Y, DISPLAY_WIDTH);
}

void drawScreenSaver(Display &display)
{
    constexpr int LOGO_GAP = 5;
    constexpr int LOGO_BOX_PADDING = 8;
    constexpr int LOGO_BOX_HEIGHT = 30;
    constexpr int LOGO_CORNER_RADIUS = 5;
    constexpr int LOGO_TEXT_BASELINE_OFFSET = 21;

    display.clearBuffer();

    display.setFont(u8g2_font_helvB14_tr);

    const char *herbText = "HERB";
    const char *hubText = "HUB";

    int herbWidth = display.getStrWidth(herbText);
    int hubWidth = display.getStrWidth(hubText);

    int boxWidth = hubWidth + LOGO_BOX_PADDING * 2;
    int boxHeight = LOGO_BOX_HEIGHT;

    int totalWidth = herbWidth + LOGO_GAP + boxWidth;
    int startX = (DISPLAY_WIDTH - totalWidth) / 2;

    int boxX = startX + herbWidth + LOGO_GAP;
    int boxY = (DISPLAY_HEIGHT - boxHeight) / 2;

    // HERB
    display.setDrawColor(1);
    display.drawStr(
        startX,
        boxY + LOGO_TEXT_BASELINE_OFFSET,
        herbText);

    // HUB box
    display.drawRBox(
        boxX,
        boxY,
        boxWidth,
        boxHeight,
        LOGO_CORNER_RADIUS);

    // HUB text
    display.setDrawColor(0);

    display.drawStr(
        boxX + LOGO_BOX_PADDING,
        boxY + LOGO_TEXT_BASELINE_OFFSET,
        hubText);

    display.setDrawColor(1);

    display.sendBuffer();
}

void drawHomeScreen(
    Display &display,
    const SystemState &state)
{
    constexpr int LEFT_COLUMN_CENTER = 32;
    constexpr int RIGHT_COLUMN_CENTER = 96;
    constexpr int VALUE_BASELINE = 31;
    constexpr int LABEL_BASELINE = 42;
    constexpr int STATUS_BASELINE = 53;
    constexpr int FOOTER_BASELINE = 63;

    display.clearBuffer();

    // --------------------------------------------------
    // Header
    // --------------------------------------------------

    drawHeader(display, state, "HERB HUB");

    // --------------------------------------------------
    // Temperature / Humidity
    // --------------------------------------------------

    display.setFont(u8g2_font_ncenB10_tr);

    char tempText[20];

    if (state.settings.useFahrenheit)
    {
        sprintf(
            tempText,
            "%.1f F",
            state.sensorReadings.temperatureF);
    }
    else
    {
        sprintf(
            tempText,
            "%.1f C",
            state.sensorReadings.temperatureC);
    }

    char humidityText[20];

    sprintf(
        humidityText,
        "%.1f %%",
        state.sensorReadings.humidity);

    // Center temperature in left half
    int tempWidth = display.getStrWidth(tempText);
    int leftColumnWidth = LEFT_COLUMN_CENTER * 2;
    int tempX = (leftColumnWidth - tempWidth) / 2;

    display.drawStr(
        tempX,
        VALUE_BASELINE,
        tempText);

    // Center humidity in right half
    int humidityWidth = display.getStrWidth(humidityText);
    int humidityX = RIGHT_COLUMN_CENTER - humidityWidth / 2;

    display.drawStr(
        humidityX,
        VALUE_BASELINE,
        humidityText);

    // Small labels
    display.setFont(u8g2_font_6x10_tr);

    const char *tempLabel = "TEMP";
    const char *humidityLabel = "RH";

    int tempLabelWidth = display.getStrWidth(tempLabel);
    int humidityLabelWidth = display.getStrWidth(humidityLabel);
    int tempLabelX = (leftColumnWidth - tempLabelWidth) / 2;
    int humidityLabelX = RIGHT_COLUMN_CENTER - humidityLabelWidth / 2;

    display.drawStr(
        tempLabelX,
        LABEL_BASELINE,
        tempLabel);

    display.drawStr(
        humidityLabelX,
        LABEL_BASELINE,
        humidityLabel);

    // --------------------------------------------------
    // Water level
    // --------------------------------------------------

    const char *waterText =
        state.sensorReadings.floatClosed
            ? "WATER OK"
            : "WATER LOW";

    int waterWidth = display.getStrWidth(waterText);
    int waterX = (DISPLAY_WIDTH - waterWidth) / 2;

    display.drawStr(
        waterX,
        STATUS_BASELINE,
        waterText);

    // --------------------------------------------------
    // Next watering time
    // --------------------------------------------------

    if (state.settings.modeAuto)
    {
        const char *nextWaterText = "Next: --:--";

        int nextWidth = display.getStrWidth(nextWaterText);
        int nextX = (DISPLAY_WIDTH - nextWidth) / 2;

        display.drawStr(
            nextX,
            FOOTER_BASELINE,
            nextWaterText);
    }

    display.sendBuffer();
}

void drawLightsScreen(
    Display &display,
    const SystemState &state,
    const UIState &uiState)
{
    constexpr int TIME_COLON_OFFSET = 1;
    constexpr int TIME_MINUTE_OFFSET = 2;
    constexpr int MINUTE_TEXT_OFFSET = 3;
    constexpr int TIME_COLUMN_COUNT = 2;
    constexpr int HOUR_FIELD = 0;
    constexpr int MINUTE_FIELD = 1;
    constexpr int COLUMN_STEP = 43;
    constexpr int COLUMN_WIDTH = 42;
    constexpr int SELECTION_TOP = 21;
    constexpr int SELECTION_HEIGHT = 18;
    constexpr int EDIT_TOP = 22;
    constexpr int EDIT_HEIGHT = 17;
    constexpr int VALUE_BASELINE = 36;
    constexpr int LABEL_BASELINE = 55;
    constexpr int SELECTION_PADDING = 3;
    constexpr int EDIT_PADDING = 2;
    constexpr int EDIT_CORNER_RADIUS = 2;

    display.clearBuffer();

    drawHeader(display, state, "LIGHTS");

    const LightSchedule &schedule =
        uiState.mode == UIMode::EDIT
            ? uiState.editSchedule
            : state.settings.lightSchedule;

    const char *columnLabels[] = {
        "BEG",
        "END",
        "FRQ"};

    char beginValue[8];
    char endValue[8];
    char frequencyValue[8];

    snprintf(
        beginValue,
        sizeof(beginValue),
        "%02d:%02d",
        schedule.startHour,
        schedule.startMinute);

    snprintf(
        endValue,
        sizeof(endValue),
        "%02d:%02d",
        schedule.endHour,
        schedule.endMinute);

    snprintf(
        frequencyValue,
        sizeof(frequencyValue),
        "%dd",
        schedule.frequencyDays);

    const char *columnValues[] = {
        beginValue,
        endValue,
        frequencyValue};

    for (int column = 0; column < LIGHT_OPTION_COUNT; column++)
    {
        int columnLeft = column * COLUMN_STEP;
        int columnWidth = COLUMN_WIDTH;

        bool selected =
            (uiState.mode == UIMode::SELECT ||
             uiState.mode == UIMode::EDIT) &&
            uiState.selectedOption == column;

        // --------------------------------------------------
        // Value
        // --------------------------------------------------

        display.setFont(u8g2_font_7x13B_tr);

        int valueWidth =
            display.getStrWidth(columnValues[column]);

        int valueX =
            columnLeft + (columnWidth - valueWidth) / 2;

        // --------------------------------------------------
        // SELECT mode
        // --------------------------------------------------

        if (selected && uiState.mode == UIMode::SELECT)
        {
            display.setDrawColor(1);

            display.drawBox(
                valueX - SELECTION_PADDING,
                SELECTION_TOP,
                valueWidth + SELECTION_PADDING * 2,
                SELECTION_HEIGHT);

            display.setDrawColor(0);

            display.drawStr(
                valueX,
                VALUE_BASELINE,
                columnValues[column]);

            display.setDrawColor(1);
        }

        // --------------------------------------------------
        // EDIT mode
        // --------------------------------------------------

        else if (selected && uiState.mode == UIMode::EDIT)
        {
            if (column < TIME_COLUMN_COUNT)
            {
                // Draw the complete time normally first
                display.setDrawColor(1);

                display.drawStr(
                    valueX,
                    VALUE_BASELINE,
                    columnValues[column]);

                int hourWidth =
                    display.getStrWidth("00");

                int colonX =
                    valueX +
                    hourWidth +
                    TIME_COLON_OFFSET;

                int minuteX =
                    valueX +
                    display.getStrWidth("00:") +
                    TIME_MINUTE_OFFSET;

                // --------------------------------------------------
                // Highlight selected field
                // --------------------------------------------------

                int highlightX =
                    uiState.editField == HOUR_FIELD
                        ? valueX
                        : minuteX;

                display.drawRBox(
                    highlightX - EDIT_PADDING,
                    EDIT_TOP,
                    hourWidth + EDIT_PADDING * 2,
                    EDIT_HEIGHT,
                    EDIT_CORNER_RADIUS);

                // --------------------------------------------------
                // Redraw hour
                // --------------------------------------------------

                char hourText[3];

                hourText[0] = columnValues[column][0];
                hourText[1] = columnValues[column][1];
                hourText[2] = '\0';

                if (uiState.editField == HOUR_FIELD)
                {
                    // Hour selected: black on white
                    display.setDrawColor(0);
                }
                else
                {
                    // Hour not selected: white on black
                    display.setDrawColor(1);
                }

                display.drawStr(
                    valueX,
                    VALUE_BASELINE,
                    hourText);

                // --------------------------------------------------
                // Redraw minute
                // --------------------------------------------------

                if (uiState.editField == MINUTE_FIELD)
                {
                    // Minute selected: black on white
                    display.setDrawColor(0);
                }
                else
                {
                    // Minute not selected: white on black
                    display.setDrawColor(1);
                }

                display.drawStr(
                    minuteX,
                    VALUE_BASELINE,
                    columnValues[column] + MINUTE_TEXT_OFFSET);

                // --------------------------------------------------
                // Colon always normal
                // --------------------------------------------------

                display.setDrawColor(1);

                display.drawStr(
                    colonX,
                    VALUE_BASELINE,
                    ":");
            }
            else
            {
                // FRQ -- unchanged
                display.setDrawColor(1);

                display.drawRBox(
                    valueX - EDIT_PADDING,
                    EDIT_TOP,
                    valueWidth + EDIT_PADDING * 2,
                    EDIT_HEIGHT,
                    EDIT_CORNER_RADIUS);

                display.setDrawColor(0);

                display.drawStr(
                    valueX,
                    VALUE_BASELINE,
                    columnValues[column]);

                display.setDrawColor(1);
            }
        }

        // --------------------------------------------------
        // Normal VIEW mode
        // --------------------------------------------------

        else
        {
            display.setDrawColor(1);

            display.drawStr(
                valueX,
                VALUE_BASELINE,
                columnValues[column]);
        }

        // --------------------------------------------------
        // Column label
        // --------------------------------------------------

        display.setDrawColor(1);
        display.setFont(u8g2_font_6x10_tr);

        int labelWidth =
            display.getStrWidth(columnLabels[column]);

        display.drawStr(
            columnLeft + (columnWidth - labelWidth) / 2,
            LABEL_BASELINE,
            columnLabels[column]);
    }

    display.setDrawColor(1);

    display.sendBuffer();
}

void drawFansScreen(
    Display &display,
    const SystemState &state,
    const UIState &uiState)
{
    display.clearBuffer();

    drawHeader(display, state, "FANS");

    display.sendBuffer();
}