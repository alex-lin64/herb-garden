#include <unity.h>
#include <Arduino.h>

#include "../../src/Helpers/Helpers.h"

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

void setup()
{
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(test_celsius_to_fahrenheit);
    RUN_TEST(test_temperature_offset);

    UNITY_END();
}

void loop()
{
}