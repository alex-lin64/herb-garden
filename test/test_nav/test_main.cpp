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

void testHomeBackReturnsFalse()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = true;

    ui.carouselIndex = 2;
    ui.screen = CAROUSEL_SCREENS_AUTO[2];

    bool result = processInput(ui, system, InputEvent::BACK);

    TEST_ASSERT_FALSE(result);
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

void testOtherScreensReturnFalse()
{
    UIState ui;
    SystemState system;

    system.settings.modeAuto = true;

    ui.screen = Screen::LIGHTS;

    bool result = processInput(ui, system, InputEvent::ROTATE_CW);

    TEST_ASSERT_FALSE(result);
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

    RUN_TEST(testHomeBackReturnsFalse);
    RUN_TEST(testHomeRotaryPushReturnsFalse);
    RUN_TEST(testHomeConfirmReturnsFalse);

    RUN_TEST(testOtherScreensReturnFalse);

    UNITY_END();
}

void loop()
{
}