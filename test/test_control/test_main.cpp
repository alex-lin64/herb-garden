#include <unity.h>
#include <Arduino.h>
#include <Preferences.h>
#include <time.h>

#include "../../src/Control/Lights.h"
#include "../../src/Control/Lights.cpp"
#include "../../src/Control/ScheduleController.h"
#include "../../src/Control/ScheduleController.cpp"

namespace
{
    struct tm makeTime(
        int hour,
        int minute,
        int dayOfYear = 0)
    {
        struct tm localTime = {};
        localTime.tm_hour = hour;
        localTime.tm_min = minute;
        localTime.tm_yday = dayOfYear;
        return localTime;
    }

    LightSchedule testSchedule()
    {
        LightSchedule schedule;
        schedule.startHour = 8;
        schedule.startMinute = 0;
        schedule.endHour = 22;
        schedule.endMinute = 0;
        schedule.frequencyDays = 1;
        return schedule;
    }

    void clearSchedulePreferences()
    {
        Preferences preferences;
        preferences.begin("schedules", false);
        preferences.clear();
        preferences.end();
    }
}

void testLightsAreOffBeforeSchedule()
{
    LightSchedule schedule = testSchedule();

    TEST_ASSERT_FALSE(
        isLightsScheduledOn(schedule, makeTime(7, 59)));
}

void testLightsTurnOnAtScheduleStart()
{
    LightSchedule schedule = testSchedule();

    TEST_ASSERT_TRUE(
        isLightsScheduledOn(schedule, makeTime(8, 0)));
}

void testLightsRemainOnDuringSchedule()
{
    LightSchedule schedule = testSchedule();

    TEST_ASSERT_TRUE(
        isLightsScheduledOn(schedule, makeTime(12, 30)));
}

void testLightsTurnOffAtScheduleEnd()
{
    LightSchedule schedule = testSchedule();

    TEST_ASSERT_FALSE(
        isLightsScheduledOn(schedule, makeTime(22, 0)));
}

void testLightFrequencySkipsNonScheduledDays()
{
    LightSchedule schedule = testSchedule();
    schedule.frequencyDays = 2;

    TEST_ASSERT_TRUE(
        isLightsScheduledOn(schedule, makeTime(10, 0, 0)));
    TEST_ASSERT_FALSE(
        isLightsScheduledOn(schedule, makeTime(10, 0, 1)));
    TEST_ASSERT_TRUE(
        isLightsScheduledOn(schedule, makeTime(10, 0, 2)));
}

void testManualLightsIgnoreScheduleWhenOff()
{
    SystemSettings settings;
    settings.modeAuto = false;
    settings.manualLightsOn = false;
    settings.lightSchedule = testSchedule();

    TEST_ASSERT_FALSE(
        resolveLightsOn(settings, makeTime(12, 0)));
}

void testManualLightsIgnoreScheduleWhenOn()
{
    SystemSettings settings;
    settings.modeAuto = false;
    settings.manualLightsOn = true;
    settings.lightSchedule = testSchedule();

    TEST_ASSERT_TRUE(
        resolveLightsOn(settings, makeTime(2, 0)));
}

void testAutoLightsUseSchedule()
{
    SystemSettings settings;
    settings.modeAuto = true;
    settings.manualLightsOn = true;
    settings.lightSchedule = testSchedule();

    TEST_ASSERT_FALSE(
        resolveLightsOn(settings, makeTime(2, 0)));
    TEST_ASSERT_TRUE(
        resolveLightsOn(settings, makeTime(12, 0)));
}

void testScheduleControllerInitializesMissingAnchors()
{
    clearSchedulePreferences();

    SystemState state;
    ScheduleController controller;
    controller.begin(state);

    TEST_ASSERT_TRUE(state.fansScheduleAnchor == 0);
    TEST_ASSERT_TRUE(state.waterScheduleAnchor == 0);

    controller.update(state, 1000);

    TEST_ASSERT_TRUE(state.fansScheduleAnchor == 1000);
    TEST_ASSERT_TRUE(state.waterScheduleAnchor == 1000);
}

void testScheduleControllerResetsOnlyChangedScheduleAnchor()
{
    clearSchedulePreferences();

    SystemState state;
    ScheduleController controller;
    controller.begin(state);
    controller.update(state, 1000);

    state.settings.fansSchedule.durationMinutes++;
    controller.update(state, 2000);

    TEST_ASSERT_TRUE(state.fansScheduleAnchor == 2000);
    TEST_ASSERT_TRUE(state.waterScheduleAnchor == 1000);

    state.settings.waterSchedule.frequencyHours++;
    controller.update(state, 3000);

    TEST_ASSERT_TRUE(state.fansScheduleAnchor == 2000);
    TEST_ASSERT_TRUE(state.waterScheduleAnchor == 3000);
}

void testScheduleControllerRestoresStateAfterReboot()
{
    clearSchedulePreferences();

    SystemState savedState;
    savedState.settings.fansSchedule.durationMinutes = 25;
    savedState.settings.waterSchedule.frequencyHours = 18;

    {
        ScheduleController controller;
        controller.begin(savedState);
        controller.update(savedState, 12345);
    }

    SystemState restoredState;

    {
        ScheduleController controller;
        controller.begin(restoredState);

        TEST_ASSERT_EQUAL_INT(
            25,
            restoredState.settings.fansSchedule.durationMinutes);
        TEST_ASSERT_EQUAL_INT(
            18,
            restoredState.settings.waterSchedule.frequencyHours);
        TEST_ASSERT_TRUE(restoredState.fansScheduleAnchor == 12345);
        TEST_ASSERT_TRUE(restoredState.waterScheduleAnchor == 12345);
    }
}

void setup()
{
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(testLightsAreOffBeforeSchedule);
    RUN_TEST(testLightsTurnOnAtScheduleStart);
    RUN_TEST(testLightsRemainOnDuringSchedule);
    RUN_TEST(testLightsTurnOffAtScheduleEnd);
    RUN_TEST(testLightFrequencySkipsNonScheduledDays);
    RUN_TEST(testManualLightsIgnoreScheduleWhenOff);
    RUN_TEST(testManualLightsIgnoreScheduleWhenOn);
    RUN_TEST(testAutoLightsUseSchedule);

    RUN_TEST(testScheduleControllerInitializesMissingAnchors);
    RUN_TEST(testScheduleControllerResetsOnlyChangedScheduleAnchor);
    RUN_TEST(testScheduleControllerRestoresStateAfterReboot);

    UNITY_END();
}

void loop()
{
}
