#include <unity.h>
#include <Arduino.h>

#include "../../src/Helpers/Helpers.h"
#include "../../src/UI/Screens.h"

void test_celsius_to_fahrenheit()
{
    TEST_ASSERT_FLOAT_WITHIN(
        0.01f,
        32.0f,
        celsiusToFahrenheit(0.0f, 0.0f));

    TEST_ASSERT_FLOAT_WITHIN(
        0.01f,
        77.0f,
        celsiusToFahrenheit(25.0f, 0.0f));
}

void test_temperature_offset()
{
    TEST_ASSERT_FLOAT_WITHIN(
        0.01f,
        31.0f,
        celsiusToFahrenheit(0.0f, -1.0f));
}

void test_next_watering_label_includes_date()
{
    struct tm nextWateringTime = {};
    nextWateringTime.tm_year = 124;
    nextWateringTime.tm_mon = 9;
    nextWateringTime.tm_mday = 19;
    nextWateringTime.tm_hour = 14;
    nextWateringTime.tm_min = 30;

    char nextWateringText[32];
    formatNextWateringText(
        mktime(&nextWateringTime),
        nextWateringText,
        sizeof(nextWateringText));

    TEST_ASSERT_EQUAL_STRING("Next: 14:30 10/19", nextWateringText);
}

void setup()
{
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(test_celsius_to_fahrenheit);
    RUN_TEST(test_temperature_offset);
    RUN_TEST(test_next_watering_label_includes_date);

    UNITY_END();
}

void loop()
{
}