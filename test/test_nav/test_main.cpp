#include <unity.h>
#include <Arduino.h>

#include "../../src/UI/uiState.h"
#include "../../src/State/State.h"
#include "../../src/UI/Navigation.cpp"

void testCarouselRotateCW()
{
    UIState ui;
    SystemState system;

    ui.carouselIndex = 0;
    ui.screen = CAROUSEL_SCREENS[0];

    processInput(ui, system, InputEvent::ROTATE_CW);

    TEST_ASSERT_EQUAL_INT(1, ui.carouselIndex);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(CAROUSEL_SCREENS[1]),
        static_cast<int>(ui.screen));
}

void testCarouselRotateCCW()
{
    UIState ui;
    SystemState system;

    ui.carouselIndex = 1;
    ui.screen = CAROUSEL_SCREENS[1];

    processInput(ui, system, InputEvent::ROTATE_CCW);

    TEST_ASSERT_EQUAL_INT(0, ui.carouselIndex);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(CAROUSEL_SCREENS[0]),
        static_cast<int>(ui.screen));
}

void testCarouselRotateCWWraps()
{
    UIState ui;
    SystemState system;

    ui.carouselIndex = CAROUSEL_SCREEN_COUNT - 1;
    ui.screen = CAROUSEL_SCREENS[ui.carouselIndex];

    processInput(ui, system, InputEvent::ROTATE_CW);

    TEST_ASSERT_EQUAL_INT(0, ui.carouselIndex);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(CAROUSEL_SCREENS[0]),
        static_cast<int>(ui.screen));
}

void testCarouselRotateCCWWraps()
{
    UIState ui;
    SystemState system;

    ui.carouselIndex = 0;
    ui.screen = CAROUSEL_SCREENS[0];

    processInput(ui, system, InputEvent::ROTATE_CCW);

    TEST_ASSERT_EQUAL_INT(
        CAROUSEL_SCREEN_COUNT - 1,
        ui.carouselIndex);

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(
            CAROUSEL_SCREENS[CAROUSEL_SCREEN_COUNT - 1]),
        static_cast<int>(ui.screen));
}

void testCarouselBackReturnsToFirstPage()
{
    UIState ui;
    SystemState system;

    ui.carouselIndex = 3;
    ui.screen = CAROUSEL_SCREENS[3];

    processInput(ui, system, InputEvent::BACK);

    TEST_ASSERT_EQUAL_INT(0, ui.carouselIndex);
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(CAROUSEL_SCREENS[0]),
        static_cast<int>(ui.screen));
}

void setup()
{
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(testCarouselRotateCW);
    RUN_TEST(testCarouselRotateCCW);
    RUN_TEST(testCarouselRotateCWWraps);
    RUN_TEST(testCarouselRotateCCWWraps);
    RUN_TEST(testCarouselBackReturnsToFirstPage);

    UNITY_END();
}

void loop()
{
}