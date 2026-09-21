#include <unity.h>
#include <Arduino.h>

#include "../../src/UI/uiState.h"
#include "../../src/State/State.h"
#include "../../src/UI/Navigation.cpp"

void testHomeAutoRotateCW()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = true;

    ui.carouselIndex = 0;
    ui.screen = Screen::HOME;

    bool result = processInput(ui, system, InputEvent::ROTATE_CW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(1, ui.carouselIndex);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(CAROUSEL_SCREENS_AUTO[1]),
        static_cast<int>(ui.screen));
}

void testHomeAutoRotateCCW()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = true;

    ui.carouselIndex = 1;
    ui.screen = CAROUSEL_SCREENS_AUTO[1];

    bool result = processInput(ui, system, InputEvent::ROTATE_CCW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(0, ui.carouselIndex);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(CAROUSEL_SCREENS_AUTO[0]),
        static_cast<int>(ui.screen));
}

void testHomeAutoRotateCWWraps()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = true;

    ui.carouselIndex = CAROUSEL_SCREEN_COUNT_AUTO - 1;
    ui.screen = CAROUSEL_SCREENS_AUTO[ui.carouselIndex];

    bool result = processInput(ui, system, InputEvent::ROTATE_CW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(0, ui.carouselIndex);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(CAROUSEL_SCREENS_AUTO[0]),
        static_cast<int>(ui.screen));
}

void testHomeAutoRotateCCWWraps()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = true;

    ui.carouselIndex = 0;
    ui.screen = CAROUSEL_SCREENS_AUTO[0];

    bool result = processInput(ui, system, InputEvent::ROTATE_CCW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(
        CAROUSEL_SCREEN_COUNT_AUTO - 1,
        ui.carouselIndex);

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(
            CAROUSEL_SCREENS_AUTO[CAROUSEL_SCREEN_COUNT_AUTO - 1]),
        static_cast<int>(ui.screen));
}

void testHomeManualRotateCW()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = false;

    ui.carouselIndex = 0;
    ui.screen = Screen::HOME;

    bool result = processInput(ui, system, InputEvent::ROTATE_CW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(1, ui.carouselIndex);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(CAROUSEL_SCREENS_MANUAL[1]),
        static_cast<int>(ui.screen));
}

void testHomeManualRotateCCW()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = false;

    ui.carouselIndex = 1;
    ui.screen = CAROUSEL_SCREENS_MANUAL[1];

    bool result = processInput(ui, system, InputEvent::ROTATE_CCW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(0, ui.carouselIndex);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(CAROUSEL_SCREENS_MANUAL[0]),
        static_cast<int>(ui.screen));
}

void testHomeManualRotateCWWraps()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = false;

    ui.carouselIndex = CAROUSEL_SCREEN_COUNT_MANUAL - 1;
    ui.screen = CAROUSEL_SCREENS_MANUAL[ui.carouselIndex];

    bool result = processInput(ui, system, InputEvent::ROTATE_CW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(0, ui.carouselIndex);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(CAROUSEL_SCREENS_MANUAL[0]),
        static_cast<int>(ui.screen));
}

void testHomeManualRotateCCWWraps()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = false;

    ui.carouselIndex = 0;
    ui.screen = CAROUSEL_SCREENS_MANUAL[0];

    bool result = processInput(ui, system, InputEvent::ROTATE_CCW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(
        CAROUSEL_SCREEN_COUNT_MANUAL - 1,
        ui.carouselIndex);

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(
            CAROUSEL_SCREENS_MANUAL[CAROUSEL_SCREEN_COUNT_MANUAL - 1]),
        static_cast<int>(ui.screen));
}

void testHomeBackEntersScreenSaver()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = true;

    ui.screen = Screen::HOME;

    bool result = processInput(ui, system, InputEvent::BACK);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(Screen::SCREEN_SAVER),
        static_cast<int>(ui.screen));
}

void testScreenSaverInputReturnsHome()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::SCREEN_SAVER;
    ui.carouselIndex = 2;

    bool result = processInput(ui, system, InputEvent::ROTARY_PUSH);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(Screen::HOME),
        static_cast<int>(ui.screen));
    TEST_ASSERT_EQUAL_INT(0, ui.carouselIndex);
}

void testHomeRotaryPushReturnsFalse()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = true;

    ui.carouselIndex = 0;
    ui.screen = Screen::HOME;

    bool result = processInput(ui, system, InputEvent::ROTARY_PUSH);

    TEST_ASSERT_FALSE(result);
}

void testHomeConfirmReturnsFalse()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = true;

    ui.carouselIndex = 0;
    ui.screen = Screen::HOME;

    bool result = processInput(ui, system, InputEvent::CONFIRM);

    TEST_ASSERT_FALSE(result);
}

void testLightsViewEntersSelectMode()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::LIGHTS;
    ui.mode = UIMode::VIEW;
    ui.selectedOption = 2;

    bool result = processInput(ui, system, InputEvent::ROTARY_PUSH);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::SELECT),
        static_cast<int>(ui.mode));
    TEST_ASSERT_EQUAL_INT(0, ui.selectedOption);
}

void testLightsViewBackReturnsHome()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::LIGHTS;
    ui.mode = UIMode::VIEW;
    ui.carouselIndex = 1;

    bool result = processInput(ui, system, InputEvent::BACK);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(Screen::HOME),
        static_cast<int>(ui.screen));
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::VIEW),
        static_cast<int>(ui.mode));
    TEST_ASSERT_EQUAL_INT(0, ui.carouselIndex);
}

void testLightsSelectWrapsOptions()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::LIGHTS;
    ui.mode = UIMode::SELECT;
    ui.selectedOption = 0;

    bool result = processInput(ui, system, InputEvent::ROTATE_CCW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(LIGHT_OPTION_COUNT - 1, ui.selectedOption);

    result = processInput(ui, system, InputEvent::ROTATE_CW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(0, ui.selectedOption);
}

void testLightsSelectCopiesScheduleForEditing()
{
    UIState ui;
    SystemState system;

    system.settings.lightSchedule.startHour = 8;
    system.settings.lightSchedule.startMinute = 15;
    system.settings.lightSchedule.endHour = 22;
    system.settings.lightSchedule.endMinute = 45;

    ui.screen = Screen::LIGHTS;
    ui.mode = UIMode::SELECT;

    bool result = processInput(ui, system, InputEvent::CONFIRM);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::EDIT),
        static_cast<int>(ui.mode));
    TEST_ASSERT_EQUAL_INT(0, ui.editField);
    TEST_ASSERT_EQUAL_INT(8, ui.editLightSchedule.startHour);
    TEST_ASSERT_EQUAL_INT(15, ui.editLightSchedule.startMinute);
    TEST_ASSERT_EQUAL_INT(22, ui.editLightSchedule.endHour);
    TEST_ASSERT_EQUAL_INT(45, ui.editLightSchedule.endMinute);
}

void testLightsEditWrapsStartHour()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::LIGHTS;
    ui.mode = UIMode::EDIT;
    ui.selectedOption = 0;
    ui.editField = 0;
    ui.editLightSchedule.startHour = 23;

    bool result = processInput(ui, system, InputEvent::ROTATE_CW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(0, ui.editLightSchedule.startHour);

    result = processInput(ui, system, InputEvent::ROTATE_CCW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(23, ui.editLightSchedule.startHour);
}

void testLightsEditWrapsEndMinute()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::LIGHTS;
    ui.mode = UIMode::EDIT;
    ui.selectedOption = 1;
    ui.editField = 1;
    ui.editLightSchedule.endMinute = 59;

    bool result = processInput(ui, system, InputEvent::ROTATE_CW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(0, ui.editLightSchedule.endMinute);

    result = processInput(ui, system, InputEvent::ROTATE_CCW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(59, ui.editLightSchedule.endMinute);
}

void testLightsEditConfirmMovesFromHourToMinute()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::LIGHTS;
    ui.mode = UIMode::EDIT;
    ui.selectedOption = 0;
    ui.editField = 0;

    bool result = processInput(ui, system, InputEvent::CONFIRM);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(1, ui.editField);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::EDIT),
        static_cast<int>(ui.mode));
}

void testLightsEditConfirmCommitsSchedule()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::LIGHTS;
    ui.mode = UIMode::EDIT;
    ui.selectedOption = 0;
    ui.editField = 1;
    ui.editLightSchedule.startHour = 7;
    ui.editLightSchedule.startMinute = 30;

    bool result = processInput(ui, system, InputEvent::CONFIRM);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(7, system.settings.lightSchedule.startHour);
    TEST_ASSERT_EQUAL_INT(30, system.settings.lightSchedule.startMinute);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::SELECT),
        static_cast<int>(ui.mode));
    TEST_ASSERT_EQUAL_INT(0, ui.editField);
}

void testLightsEditBackCancelsEditing()
{
    UIState ui;
    SystemState system;

    system.settings.lightSchedule.startHour = 8;

    ui.screen = Screen::LIGHTS;
    ui.mode = UIMode::EDIT;
    ui.editField = 1;
    ui.editLightSchedule.startHour = 12;

    bool result = processInput(ui, system, InputEvent::BACK);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(8, system.settings.lightSchedule.startHour);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::SELECT),
        static_cast<int>(ui.mode));
    TEST_ASSERT_EQUAL_INT(0, ui.editField);
}

void testLightsEditRejectsEqualStartAndEndTimes()
{
    UIState ui;
    SystemState system;

    system.settings.lightSchedule.startHour = 8;
    system.settings.lightSchedule.startMinute = 0;
    system.settings.lightSchedule.endHour = 22;
    system.settings.lightSchedule.endMinute = 0;

    ui.screen = Screen::LIGHTS;
    ui.mode = UIMode::EDIT;
    ui.selectedOption = 1;
    ui.editField = 1;
    ui.editLightSchedule = system.settings.lightSchedule;
    ui.editLightSchedule.endHour = 8;
    ui.editLightSchedule.endMinute = 0;

    bool result = processInput(ui, system, InputEvent::CONFIRM);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(Screen::ERROR),
        static_cast<int>(ui.screen));
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(Screen::LIGHTS),
        static_cast<int>(ui.errorReturnScreen));
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::SELECT),
        static_cast<int>(ui.errorReturnMode));
    TEST_ASSERT_EQUAL_INT(1, ui.selectedOption);
    TEST_ASSERT_EQUAL_INT(22, system.settings.lightSchedule.endHour);
    TEST_ASSERT_EQUAL_INT(0, system.settings.lightSchedule.endMinute);
}

void testLightsEditRejectsEndBeforeStart()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::LIGHTS;
    ui.mode = UIMode::EDIT;
    ui.selectedOption = 1;
    ui.editField = 1;
    ui.editLightSchedule.startHour = 18;
    ui.editLightSchedule.startMinute = 30;
    ui.editLightSchedule.endHour = 7;
    ui.editLightSchedule.endMinute = 0;

    bool result = processInput(ui, system, InputEvent::CONFIRM);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(Screen::ERROR),
        static_cast<int>(ui.screen));
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(Screen::LIGHTS),
        static_cast<int>(ui.errorReturnScreen));
    TEST_ASSERT_EQUAL_INT(1, ui.selectedOption);
}

void testErrorInputRestoresPreviousContext()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::ERROR;
    ui.errorReturnScreen = Screen::LIGHTS;
    ui.errorReturnMode = UIMode::VIEW;
    ui.errorReturnSelectedOption = 1;
    ui.errorReturnEditField = HOUR_FIELD;

    bool result = processInput(ui, system, InputEvent::ROTARY_PUSH);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(Screen::LIGHTS),
        static_cast<int>(ui.screen));
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::VIEW),
        static_cast<int>(ui.mode));
    TEST_ASSERT_EQUAL_INT(1, ui.selectedOption);
    TEST_ASSERT_EQUAL_INT(HOUR_FIELD, ui.editField);
}

void testFansViewEntersSelectModeAndResetsOption()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::FANS;
    ui.mode = UIMode::VIEW;
    ui.selectedOption = 1;

    bool result = processInput(ui, system, InputEvent::CONFIRM);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::SELECT),
        static_cast<int>(ui.mode));
    TEST_ASSERT_EQUAL_INT(0, ui.selectedOption);
}

void testFansSelectWrapsOptions()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::FANS;
    ui.mode = UIMode::SELECT;
    ui.selectedOption = 0;

    bool result = processInput(ui, system, InputEvent::ROTATE_CCW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(DURATION_OPTION_COUNT - 1, ui.selectedOption);

    result = processInput(ui, system, InputEvent::ROTATE_CW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(0, ui.selectedOption);
}

void testFansSelectCopiesScheduleForEditing()
{
    UIState ui;
    SystemState system;

    system.settings.fansSchedule.durationMinutes = 10;
    system.settings.fansSchedule.frequencyHours = 1;

    ui.screen = Screen::FANS;
    ui.mode = UIMode::SELECT;

    bool result = processInput(ui, system, InputEvent::CONFIRM);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::EDIT),
        static_cast<int>(ui.mode));
    TEST_ASSERT_EQUAL_INT(0, ui.editField);
    TEST_ASSERT_EQUAL_INT(10, ui.editDurationSchedule.durationMinutes);
    TEST_ASSERT_EQUAL_INT(1, ui.editDurationSchedule.frequencyHours);
}

void testFansEditWrapsDurationMinutesThrough99()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::FANS;
    ui.mode = UIMode::EDIT;
    ui.selectedOption = 0;
    ui.editField = 0;
    ui.editDurationSchedule.durationMinutes = 99;

    bool result = processInput(ui, system, InputEvent::ROTATE_CW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(1, ui.editDurationSchedule.durationMinutes);

    result = processInput(ui, system, InputEvent::ROTATE_CCW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(99, ui.editDurationSchedule.durationMinutes);
}

void testFansEditWrapsFrequencyHoursThrough99()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::FANS;
    ui.mode = UIMode::EDIT;
    ui.selectedOption = 1;
    ui.editDurationSchedule.frequencyHours = 99;

    bool result = processInput(ui, system, InputEvent::ROTATE_CW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(1, ui.editDurationSchedule.frequencyHours);

    result = processInput(ui, system, InputEvent::ROTATE_CCW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(99, ui.editDurationSchedule.frequencyHours);
}

void testFansEditConfirmCommitsImmediately()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::FANS;
    ui.mode = UIMode::EDIT;
    ui.selectedOption = 0;
    ui.editDurationSchedule.durationMinutes = 25;

    bool result = processInput(ui, system, InputEvent::CONFIRM);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(25, system.settings.fansSchedule.durationMinutes);
    TEST_ASSERT_EQUAL_INT(static_cast<int>(UIMode::SELECT), static_cast<int>(ui.mode));
}

void testFansEditConfirmCommitsSchedule()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::FANS;
    ui.mode = UIMode::EDIT;
    ui.selectedOption = 1;
    ui.editField = 0;
    ui.editDurationSchedule.durationMinutes = 10;
    ui.editDurationSchedule.frequencyHours = 2;

    bool result = processInput(ui, system, InputEvent::CONFIRM);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(10, system.settings.fansSchedule.durationMinutes);
    TEST_ASSERT_EQUAL_INT(2, system.settings.fansSchedule.frequencyHours);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::SELECT),
        static_cast<int>(ui.mode));
    TEST_ASSERT_EQUAL_INT(0, ui.editField);
}

void testManualHomeRotatesToLightsAndWaterScreens()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = false;
    ui.screen = Screen::HOME;
    ui.carouselIndex = 0;

    bool result = processInput(ui, system, InputEvent::ROTATE_CW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(1, ui.carouselIndex);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(Screen::LIGHTS_MANUAL),
        static_cast<int>(ui.screen));

    result = processInput(ui, system, InputEvent::ROTATE_CW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(2, ui.carouselIndex);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(Screen::FANS_MANUAL),
        static_cast<int>(ui.screen));

    result = processInput(ui, system, InputEvent::ROTATE_CW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(3, ui.carouselIndex);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(Screen::WATER_MANUAL),
        static_cast<int>(ui.screen));
}

void testManualLightsSelectStartsAtCurrentOffSetting()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = false;
    system.settings.manualLightsOn = false;
    ui.screen = Screen::LIGHTS_MANUAL;
    ui.mode = UIMode::VIEW;

    bool result = processInput(ui, system, InputEvent::CONFIRM);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::SELECT),
        static_cast<int>(ui.mode));
    TEST_ASSERT_EQUAL_INT(1, ui.selectedOption);
}

void testManualLightsSelectStartsAtCurrentOnSetting()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = false;
    system.settings.manualLightsOn = true;
    ui.screen = Screen::LIGHTS_MANUAL;
    ui.mode = UIMode::VIEW;

    bool result = processInput(ui, system, InputEvent::ROTARY_PUSH);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(0, ui.selectedOption);
}

void testManualLightsSelectCommitsOn()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = false;
    system.settings.manualLightsOn = false;
    ui.screen = Screen::LIGHTS_MANUAL;
    ui.mode = UIMode::SELECT;
    ui.selectedOption = 1;

    bool result = processInput(ui, system, InputEvent::ROTATE_CCW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(0, ui.selectedOption);

    result = processInput(ui, system, InputEvent::CONFIRM);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_TRUE(system.settings.manualLightsOn);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::VIEW),
        static_cast<int>(ui.mode));
}

void testManualLightsSelectBackCancels()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = false;
    system.settings.manualLightsOn = false;
    ui.screen = Screen::LIGHTS_MANUAL;
    ui.mode = UIMode::SELECT;
    ui.selectedOption = 0;

    bool result = processInput(ui, system, InputEvent::BACK);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_FALSE(system.settings.manualLightsOn);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::VIEW),
        static_cast<int>(ui.mode));
}

void testManualWaterViewUsesCarouselNavigation()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = false;
    ui.screen = Screen::WATER_MANUAL;
    ui.mode = UIMode::VIEW;
    ui.carouselIndex = 3;

    bool result = processInput(ui, system, InputEvent::ROTATE_CW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(4, ui.carouselIndex);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(Screen::MODE),
        static_cast<int>(ui.screen));
}

void testManualWaterSelectDoesNotEdit()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = false;
    ui.screen = Screen::WATER_MANUAL;
    ui.mode = UIMode::SELECT;

    bool result = processInput(ui, system, InputEvent::ROTATE_CW);

    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::SELECT),
        static_cast<int>(ui.mode));
}

void testWaterSelectCopiesScheduleForEditing()
{
    UIState ui;
    SystemState system;

    system.settings.waterSchedule.durationMinutes = 5;
    system.settings.waterSchedule.frequencyHours = 12;

    ui.screen = Screen::WATER;
    ui.mode = UIMode::SELECT;

    bool result = processInput(ui, system, InputEvent::CONFIRM);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(static_cast<int>(UIMode::EDIT), static_cast<int>(ui.mode));
    TEST_ASSERT_EQUAL_INT(5, ui.editDurationSchedule.durationMinutes);
    TEST_ASSERT_EQUAL_INT(12, ui.editDurationSchedule.frequencyHours);
}

void testWaterEditCommitsOnlyWaterSchedule()
{
    UIState ui;
    SystemState system;

    system.settings.fansSchedule.durationMinutes = 10;
    system.settings.waterSchedule.durationMinutes = 5;

    ui.screen = Screen::WATER;
    ui.mode = UIMode::EDIT;
    ui.selectedOption = 0;
    ui.editDurationSchedule.durationMinutes = 25;
    ui.editDurationSchedule.frequencyHours = 18;

    bool result = processInput(ui, system, InputEvent::CONFIRM);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(25, system.settings.waterSchedule.durationMinutes);
    TEST_ASSERT_EQUAL_INT(18, system.settings.waterSchedule.frequencyHours);
    TEST_ASSERT_EQUAL_INT(10, system.settings.fansSchedule.durationMinutes);
}

void testModeViewEntersSelectAtCurrentSetting()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = false;
    ui.screen = Screen::MODE;
    ui.mode = UIMode::VIEW;

    bool result = processInput(ui, system, InputEvent::CONFIRM);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(static_cast<int>(UIMode::SELECT), static_cast<int>(ui.mode));
    TEST_ASSERT_EQUAL_INT(1, ui.selectedOption);
}

void testModeSelectMovesAndCommitsManual()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = true;
    ui.screen = Screen::MODE;
    ui.mode = UIMode::SELECT;
    ui.selectedOption = 0;

    bool result = processInput(ui, system, InputEvent::ROTATE_CW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(1, ui.selectedOption);

    result = processInput(ui, system, InputEvent::CONFIRM);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_FALSE(system.settings.modeAuto);
    TEST_ASSERT_EQUAL_INT(static_cast<int>(UIMode::VIEW), static_cast<int>(ui.mode));
}

void testModeSelectWrapsBetweenAutoAndManual()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::MODE;
    ui.mode = UIMode::SELECT;
    ui.selectedOption = 0;

    bool result = processInput(ui, system, InputEvent::ROTATE_CCW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(1, ui.selectedOption);

    result = processInput(ui, system, InputEvent::ROTATE_CW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(0, ui.selectedOption);
}

void testModeSelectBackCancelsWithoutChangingSetting()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = true;
    ui.screen = Screen::MODE;
    ui.mode = UIMode::SELECT;
    ui.selectedOption = 1;

    bool result = processInput(ui, system, InputEvent::BACK);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_TRUE(system.settings.modeAuto);
    TEST_ASSERT_EQUAL_INT(static_cast<int>(UIMode::VIEW), static_cast<int>(ui.mode));
}

void testLightsEditNoneDoesNothing()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::LIGHTS;
    ui.mode = UIMode::EDIT;
    ui.selectedOption = 0;
    ui.editField = HOUR_FIELD;
    ui.editLightSchedule.startHour = 10;

    bool result = processInput(
        ui,
        system,
        InputEvent::NONE);

    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL_INT(
        10,
        ui.editLightSchedule.startHour);
    TEST_ASSERT_EQUAL_INT(
        HOUR_FIELD,
        ui.editField);
}

void testLightsSelectNoneDoesNothing()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::LIGHTS;
    ui.mode = UIMode::SELECT;
    ui.selectedOption = 1;

    bool result = processInput(
        ui,
        system,
        InputEvent::NONE);

    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL_INT(1, ui.selectedOption);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::SELECT),
        static_cast<int>(ui.mode));
}

void testModeViewRotaryPushUsesCurrentSetting()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = true;

    ui.screen = Screen::MODE;
    ui.mode = UIMode::VIEW;
    ui.selectedOption = 1;

    bool result = processInput(
        ui,
        system,
        InputEvent::ROTARY_PUSH);

    TEST_ASSERT_TRUE(result);

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::SELECT),
        static_cast<int>(ui.mode));

    TEST_ASSERT_EQUAL_INT(0, ui.selectedOption);
}

void testModeSelectCommitsAuto()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = false;

    ui.screen = Screen::MODE;
    ui.mode = UIMode::SELECT;
    ui.selectedOption = 0;

    bool result = processInput(
        ui,
        system,
        InputEvent::CONFIRM);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_TRUE(system.settings.modeAuto);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::VIEW),
        static_cast<int>(ui.mode));
}

void testManualWaterSelectBackCancels()
{
    UIState ui;
    SystemState system;

    system.settings.manualWaterOn = true;

    ui.screen = Screen::WATER_MANUAL;
    ui.mode = UIMode::SELECT;
    ui.selectedOption = 0;

    bool result = processInput(
        ui,
        system,
        InputEvent::BACK);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_TRUE(system.settings.manualWaterOn);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::VIEW),
        static_cast<int>(ui.mode));
}

void testManualWaterSelectCommitsOff()
{
    UIState ui;
    SystemState system;

    system.settings.manualWaterOn = true;

    ui.screen = Screen::WATER_MANUAL;
    ui.mode = UIMode::SELECT;
    ui.selectedOption = 1;

    bool result = processInput(
        ui,
        system,
        InputEvent::CONFIRM);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_FALSE(system.settings.manualWaterOn);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::VIEW),
        static_cast<int>(ui.mode));
}

void testManualWaterSelectStartsAtCurrentOnSetting()
{
    UIState ui;
    SystemState system;

    system.settings.manualWaterOn = true;

    ui.screen = Screen::WATER_MANUAL;
    ui.mode = UIMode::VIEW;

    bool result = processInput(
        ui,
        system,
        InputEvent::ROTARY_PUSH);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(0, ui.selectedOption);
}

void testManualWaterSelectStartsAtCurrentOffSetting()
{
    UIState ui;
    SystemState system;

    system.settings.manualWaterOn = false;

    ui.screen = Screen::WATER_MANUAL;
    ui.mode = UIMode::VIEW;

    bool result = processInput(
        ui,
        system,
        InputEvent::CONFIRM);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::SELECT),
        static_cast<int>(ui.mode));
    TEST_ASSERT_EQUAL_INT(1, ui.selectedOption);
}

void testManualFansSelectStartsAtCurrentOffSetting()
{
    UIState ui;
    SystemState system;

    system.settings.manualFansOn = false;

    ui.screen = Screen::FANS_MANUAL;
    ui.mode = UIMode::VIEW;

    bool result = processInput(
        ui,
        system,
        InputEvent::CONFIRM);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::SELECT),
        static_cast<int>(ui.mode));
    TEST_ASSERT_EQUAL_INT(1, ui.selectedOption);
}
void testManualFansSelectStartsAtCurrentOnSetting()
{
    UIState ui;
    SystemState system;

    system.settings.manualFansOn = true;

    ui.screen = Screen::FANS_MANUAL;
    ui.mode = UIMode::VIEW;

    bool result = processInput(
        ui,
        system,
        InputEvent::ROTARY_PUSH);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(0, ui.selectedOption);
}
void testManualFansSelectCommitsOff()
{
    UIState ui;
    SystemState system;

    system.settings.manualFansOn = true;

    ui.screen = Screen::FANS_MANUAL;
    ui.mode = UIMode::SELECT;
    ui.selectedOption = 1;

    bool result = processInput(
        ui,
        system,
        InputEvent::CONFIRM);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_FALSE(system.settings.manualFansOn);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::VIEW),
        static_cast<int>(ui.mode));
}

void testManualFansSelectBackCancels()
{
    UIState ui;
    SystemState system;

    system.settings.manualFansOn = true;

    ui.screen = Screen::FANS_MANUAL;
    ui.mode = UIMode::SELECT;
    ui.selectedOption = 1;

    bool result = processInput(
        ui,
        system,
        InputEvent::BACK);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_TRUE(system.settings.manualFansOn);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::VIEW),
        static_cast<int>(ui.mode));
}

void testWaterEditBackCancelsEditing()
{
    UIState ui;
    SystemState system;

    system.settings.waterSchedule.durationMinutes = 5;
    system.settings.waterSchedule.frequencyHours = 12;

    ui.screen = Screen::WATER;
    ui.mode = UIMode::EDIT;
    ui.selectedOption = 1;

    ui.editDurationSchedule.durationMinutes = 20;
    ui.editDurationSchedule.frequencyHours = 24;

    bool result = processInput(
        ui,
        system,
        InputEvent::BACK);

    TEST_ASSERT_TRUE(result);

    TEST_ASSERT_EQUAL_INT(
        5,
        system.settings.waterSchedule.durationMinutes);

    TEST_ASSERT_EQUAL_INT(
        12,
        system.settings.waterSchedule.frequencyHours);

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::SELECT),
        static_cast<int>(ui.mode));
}

void testFansEditBackCancelsEditing()
{
    UIState ui;
    SystemState system;

    system.settings.fansSchedule.durationMinutes = 10;
    system.settings.fansSchedule.frequencyHours = 2;

    ui.screen = Screen::FANS;
    ui.mode = UIMode::EDIT;
    ui.selectedOption = 0;
    ui.editDurationSchedule.durationMinutes = 25;
    ui.editDurationSchedule.frequencyHours = 8;

    bool result = processInput(
        ui,
        system,
        InputEvent::BACK);

    TEST_ASSERT_TRUE(result);

    TEST_ASSERT_EQUAL_INT(
        10,
        system.settings.fansSchedule.durationMinutes);

    TEST_ASSERT_EQUAL_INT(
        2,
        system.settings.fansSchedule.frequencyHours);

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::SELECT),
        static_cast<int>(ui.mode));

    TEST_ASSERT_EQUAL_INT(0, ui.editField);
}

void testFansEditFrequencyWrapsMinimum()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::FANS;
    ui.mode = UIMode::EDIT;
    ui.selectedOption = 1;
    ui.editDurationSchedule.frequencyHours = 1;

    bool result = processInput(
        ui,
        system,
        InputEvent::ROTATE_CCW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(
        99,
        ui.editDurationSchedule.frequencyHours);
}

void testFansEditDurationWrapsMinimum()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::FANS;
    ui.mode = UIMode::EDIT;
    ui.selectedOption = 0;
    ui.editDurationSchedule.durationMinutes = 1;

    bool result = processInput(
        ui,
        system,
        InputEvent::ROTATE_CCW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(
        99,
        ui.editDurationSchedule.durationMinutes);
}

void testLightsSelectBackReturnsToView()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::LIGHTS;
    ui.mode = UIMode::SELECT;
    ui.selectedOption = 1;

    bool result = processInput(
        ui,
        system,
        InputEvent::BACK);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::VIEW),
        static_cast<int>(ui.mode));
    TEST_ASSERT_EQUAL_INT(1, ui.selectedOption);
}

void testLightsEditWrapsStartMinute()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::LIGHTS;
    ui.mode = UIMode::EDIT;
    ui.selectedOption = 0;
    ui.editField = MINUTE_FIELD;
    ui.editLightSchedule.startMinute = 59;

    bool result = processInput(
        ui,
        system,
        InputEvent::ROTATE_CW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(
        0,
        ui.editLightSchedule.startMinute);

    result = processInput(
        ui,
        system,
        InputEvent::ROTATE_CCW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(
        59,
        ui.editLightSchedule.startMinute);
}

void testLightsEditWrapsEndHour()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::LIGHTS;
    ui.mode = UIMode::EDIT;
    ui.selectedOption = 1;
    ui.editField = HOUR_FIELD;
    ui.editLightSchedule.endHour = 23;

    bool result = processInput(
        ui,
        system,
        InputEvent::ROTATE_CW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(
        0,
        ui.editLightSchedule.endHour);

    result = processInput(
        ui,
        system,
        InputEvent::ROTATE_CCW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(
        23,
        ui.editLightSchedule.endHour);
}

void testLightsEditRotaryPushMovesToMinute()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::LIGHTS;
    ui.mode = UIMode::EDIT;
    ui.selectedOption = 0;
    ui.editField = HOUR_FIELD;

    bool result = processInput(
        ui,
        system,
        InputEvent::ROTARY_PUSH);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(MINUTE_FIELD, ui.editField);
}

void testLightsViewRotaryPushEntersSelect()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::LIGHTS;
    ui.mode = UIMode::VIEW;

    bool result = processInput(
        ui,
        system,
        InputEvent::ROTARY_PUSH);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(UIMode::SELECT),
        static_cast<int>(ui.mode));
    TEST_ASSERT_EQUAL_INT(0, ui.selectedOption);
}

void testLightsViewRotatesToPreviousScreen()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = true;

    ui.screen = Screen::LIGHTS;
    ui.mode = UIMode::VIEW;
    ui.carouselIndex = 1;

    bool result = processInput(ui, system, InputEvent::ROTATE_CCW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(0, ui.carouselIndex);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(CAROUSEL_SCREENS_AUTO[0]),
        static_cast<int>(ui.screen));
}

void testLightsViewRotatesToNextScreen()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = true;

    ui.screen = Screen::LIGHTS;
    ui.mode = UIMode::VIEW;
    ui.carouselIndex = 1;

    bool result = processInput(ui, system, InputEvent::ROTATE_CW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(2, ui.carouselIndex);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(CAROUSEL_SCREENS_AUTO[2]),
        static_cast<int>(ui.screen));
}

void testErrorNoneDoesNothing()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::ERROR;
    ui.errorReturnScreen = Screen::LIGHTS;
    ui.errorReturnMode = UIMode::EDIT;
    ui.errorReturnSelectedOption = 1;
    ui.errorReturnEditField = 0;

    bool result = processInput(ui, system, InputEvent::NONE);

    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(Screen::ERROR),
        static_cast<int>(ui.screen));
}

void testHomeNoneReturnsFalse()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = true;
    ui.screen = Screen::HOME;
    ui.carouselIndex = 2;

    bool result = processInput(ui, system, InputEvent::NONE);

    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL_INT(2, ui.carouselIndex);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(Screen::HOME),
        static_cast<int>(ui.screen));
}

void testScreenSaverAnyInputReturnsHome()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::SCREEN_SAVER;
    ui.carouselIndex = 4;

    bool result = processInput(ui, system, InputEvent::ROTATE_CW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(Screen::HOME),
        static_cast<int>(ui.screen));
    TEST_ASSERT_EQUAL_INT(0, ui.carouselIndex);
}

void testSwitchingToManualUsesManualCarousel()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = true;

    ui.screen = Screen::MODE;
    ui.mode = UIMode::SELECT;
    ui.selectedOption = 1;

    bool result = processInput(
        ui,
        system,
        InputEvent::CONFIRM);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_FALSE(system.settings.modeAuto);

    // Now navigation should use the manual carousel.
    ui.screen = Screen::HOME;
    ui.carouselIndex = 0;

    result = processInput(
        ui,
        system,
        InputEvent::ROTATE_CW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(1, ui.carouselIndex);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(CAROUSEL_SCREENS_MANUAL[1]),
        static_cast<int>(ui.screen));
}

void testManualHomeCarouselWrapsCW()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = false;

    ui.screen = Screen::WATER_MANUAL;
    ui.carouselIndex = CAROUSEL_SCREEN_COUNT_MANUAL - 1;

    bool result = processInput(
        ui,
        system,
        InputEvent::ROTATE_CW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(0, ui.carouselIndex);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(CAROUSEL_SCREENS_MANUAL[0]),
        static_cast<int>(ui.screen));
}

void testFansEditConfirmDoesNotUseSystemUntilCommit()
{
    UIState ui;
    SystemState system;

    system.settings.fansSchedule.durationMinutes = 10;

    ui.screen = Screen::FANS;
    ui.mode = UIMode::EDIT;
    ui.selectedOption = 0;

    ui.editDurationSchedule.durationMinutes = 25;

    bool result = processInput(
        ui,
        system,
        InputEvent::NONE);

    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL_INT(
        10,
        system.settings.fansSchedule.durationMinutes);
    TEST_ASSERT_EQUAL_INT(
        25,
        ui.editDurationSchedule.durationMinutes);
}

void setup()
{
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(testHomeAutoRotateCW);
    RUN_TEST(testHomeAutoRotateCCW);
    RUN_TEST(testHomeAutoRotateCWWraps);
    RUN_TEST(testHomeAutoRotateCCWWraps);

    RUN_TEST(testHomeManualRotateCW);
    RUN_TEST(testHomeManualRotateCCW);
    RUN_TEST(testHomeManualRotateCWWraps);
    RUN_TEST(testHomeManualRotateCCWWraps);

    RUN_TEST(testHomeBackEntersScreenSaver);
    RUN_TEST(testScreenSaverInputReturnsHome);
    RUN_TEST(testHomeRotaryPushReturnsFalse);
    RUN_TEST(testHomeConfirmReturnsFalse);

    RUN_TEST(testLightsViewEntersSelectMode);
    RUN_TEST(testLightsViewBackReturnsHome);
    RUN_TEST(testLightsSelectWrapsOptions);
    RUN_TEST(testLightsSelectCopiesScheduleForEditing);
    RUN_TEST(testLightsEditWrapsStartHour);
    RUN_TEST(testLightsEditWrapsEndMinute);
    RUN_TEST(testLightsEditConfirmMovesFromHourToMinute);
    RUN_TEST(testLightsEditConfirmCommitsSchedule);
    RUN_TEST(testLightsEditBackCancelsEditing);
    RUN_TEST(testLightsEditRejectsEqualStartAndEndTimes);
    RUN_TEST(testLightsEditRejectsEndBeforeStart);
    RUN_TEST(testErrorInputRestoresPreviousContext);

    RUN_TEST(testFansViewEntersSelectModeAndResetsOption);
    RUN_TEST(testFansSelectWrapsOptions);
    RUN_TEST(testFansSelectCopiesScheduleForEditing);
    RUN_TEST(testFansEditWrapsDurationMinutesThrough99);
    RUN_TEST(testFansEditWrapsFrequencyHoursThrough99);
    RUN_TEST(testFansEditConfirmCommitsImmediately);
    RUN_TEST(testFansEditConfirmCommitsSchedule);
    RUN_TEST(testFansEditBackCancelsEditing);

    RUN_TEST(testManualHomeRotatesToLightsAndWaterScreens);
    RUN_TEST(testManualLightsSelectStartsAtCurrentOffSetting);
    RUN_TEST(testManualLightsSelectStartsAtCurrentOnSetting);
    RUN_TEST(testManualLightsSelectCommitsOn);
    RUN_TEST(testManualLightsSelectBackCancels);
    RUN_TEST(testManualWaterViewUsesCarouselNavigation);
    RUN_TEST(testManualWaterSelectDoesNotEdit);

    RUN_TEST(testWaterSelectCopiesScheduleForEditing);
    RUN_TEST(testWaterEditCommitsOnlyWaterSchedule);
    RUN_TEST(testModeViewEntersSelectAtCurrentSetting);
    RUN_TEST(testModeSelectMovesAndCommitsManual);
    RUN_TEST(testModeSelectWrapsBetweenAutoAndManual);
    RUN_TEST(testModeSelectBackCancelsWithoutChangingSetting);

    RUN_TEST(testHomeNoneReturnsFalse);
    RUN_TEST(testScreenSaverAnyInputReturnsHome);
    RUN_TEST(testErrorNoneDoesNothing);

    RUN_TEST(testLightsViewRotatesToNextScreen);
    RUN_TEST(testLightsViewRotatesToPreviousScreen);
    RUN_TEST(testLightsViewRotaryPushEntersSelect);
    RUN_TEST(testLightsSelectBackReturnsToView);

    RUN_TEST(testLightsEditWrapsStartMinute);
    RUN_TEST(testLightsEditWrapsEndHour);
    RUN_TEST(testLightsEditRotaryPushMovesToMinute);

    RUN_TEST(testFansEditDurationWrapsMinimum);
    RUN_TEST(testFansEditFrequencyWrapsMinimum);

    RUN_TEST(testWaterEditBackCancelsEditing);

    RUN_TEST(testManualFansSelectStartsAtCurrentOffSetting);
    RUN_TEST(testManualFansSelectStartsAtCurrentOnSetting);
    RUN_TEST(testManualFansSelectCommitsOff);
    RUN_TEST(testManualFansSelectBackCancels);

    RUN_TEST(testManualWaterSelectStartsAtCurrentOffSetting);
    RUN_TEST(testManualWaterSelectStartsAtCurrentOnSetting);
    RUN_TEST(testManualWaterSelectCommitsOff);
    RUN_TEST(testManualWaterSelectBackCancels);

    RUN_TEST(testModeSelectCommitsAuto);
    RUN_TEST(testModeViewRotaryPushUsesCurrentSetting);

    RUN_TEST(testLightsSelectNoneDoesNothing);
    RUN_TEST(testLightsEditNoneDoesNothing);

    RUN_TEST(testManualHomeCarouselWrapsCW);
    RUN_TEST(testSwitchingToManualUsesManualCarousel);

    UNITY_END();
}

void loop()
{
}