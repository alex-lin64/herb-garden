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

void testHomeRotaryPushReturnsFalse()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = true;

    ui.carouselIndex = 2;
    ui.screen = CAROUSEL_SCREENS_AUTO[2];

    bool result = processInput(ui, system, InputEvent::ROTARY_PUSH);

    TEST_ASSERT_FALSE(result);
}

void testHomeConfirmReturnsFalse()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = true;

    ui.carouselIndex = 2;
    ui.screen = CAROUSEL_SCREENS_AUTO[2];

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
    system.settings.lightSchedule.frequencyDays = 3;

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
    TEST_ASSERT_EQUAL_INT(3, ui.editLightSchedule.frequencyDays);
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

void testLightsEditWrapsFrequencyDays()
{
    UIState ui;
    SystemState system;

    ui.screen = Screen::LIGHTS;
    ui.mode = UIMode::EDIT;
    ui.selectedOption = 2;
    ui.editLightSchedule.frequencyDays = 9;

    bool result = processInput(ui, system, InputEvent::ROTATE_CW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(1, ui.editLightSchedule.frequencyDays);

    result = processInput(ui, system, InputEvent::ROTATE_CCW);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(9, ui.editLightSchedule.frequencyDays);
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
    ui.editLightSchedule.frequencyDays = 4;

    bool result = processInput(ui, system, InputEvent::CONFIRM);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(7, system.settings.lightSchedule.startHour);
    TEST_ASSERT_EQUAL_INT(30, system.settings.lightSchedule.startMinute);
    TEST_ASSERT_EQUAL_INT(4, system.settings.lightSchedule.frequencyDays);
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
    RUN_TEST(testHomeRotaryPushReturnsFalse);
    RUN_TEST(testHomeConfirmReturnsFalse);

    RUN_TEST(testLightsViewEntersSelectMode);
    RUN_TEST(testLightsViewBackReturnsHome);
    RUN_TEST(testLightsSelectWrapsOptions);
    RUN_TEST(testLightsSelectCopiesScheduleForEditing);
    RUN_TEST(testLightsEditWrapsStartHour);
    RUN_TEST(testLightsEditWrapsEndMinute);
    RUN_TEST(testLightsEditWrapsFrequencyDays);
    RUN_TEST(testLightsEditConfirmMovesFromHourToMinute);
    RUN_TEST(testLightsEditConfirmCommitsSchedule);
    RUN_TEST(testLightsEditBackCancelsEditing);

    UNITY_END();
}

void loop()
{
}