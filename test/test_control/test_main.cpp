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
        return schedule;
    }

    DurationSchedule testDurationSchedule()
    {
        DurationSchedule schedule;
        schedule.durationMinutes = 10;
        schedule.frequencyHours = 30;
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

void testDurationScheduleIsOffBeforeAnchor()
{
    DurationSchedule schedule = testDurationSchedule();

    TEST_ASSERT_FALSE(
        isDurationScheduleOn(schedule, 1000, 999));
}

void testDurationScheduleIsOnAtAnchor()
{
    DurationSchedule schedule = testDurationSchedule();

    TEST_ASSERT_TRUE(
        isDurationScheduleOn(schedule, 1000, 1000));
}

void testDurationScheduleTurnsOffAfterDuration()
{
    DurationSchedule schedule = testDurationSchedule();

    TEST_ASSERT_TRUE(
        isDurationScheduleOn(schedule, 1000, 1000 + 9 * 60));
    TEST_ASSERT_FALSE(
        isDurationScheduleOn(schedule, 1000, 1000 + 10 * 60));
}

void testDurationScheduleRepeatsAfterFrequency()
{
    DurationSchedule schedule = testDurationSchedule();
    time_t nextAnchor = 1000 + 30 * 60 * 60;

    TEST_ASSERT_TRUE(
        isDurationScheduleOn(schedule, 1000, nextAnchor));
    TEST_ASSERT_FALSE(
        isDurationScheduleOn(schedule, 1000, nextAnchor + 10 * 60));
}

void testDurationScheduleRejectsInvalidValues()
{
    DurationSchedule schedule = testDurationSchedule();

    schedule.frequencyHours = 0;
    TEST_ASSERT_FALSE(
        isDurationScheduleOn(schedule, 1000, 1000));

    schedule = testDurationSchedule();
    schedule.durationMinutes = 0;
    TEST_ASSERT_FALSE(
        isDurationScheduleOn(schedule, 1000, 1000));
}

void testScheduleControllerSetsAutomaticFansState()
{
    clearSchedulePreferences();

    SystemState state;
    state.settings.modeAuto = true;
    state.settings.fansSchedule = testDurationSchedule();

    ScheduleController controller;
    controller.begin(state);
    controller.update(state, 1000);

    TEST_ASSERT_TRUE(state.fansOn);

    controller.update(state, 1000 + 10 * 60);

    TEST_ASSERT_FALSE(state.fansOn);
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
    savedState.settings.lightSchedule.startHour = 7;
    savedState.settings.lightSchedule.startMinute = 30;
    savedState.settings.lightSchedule.endHour = 21;
    savedState.settings.lightSchedule.endMinute = 45;
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
        TEST_ASSERT_EQUAL_INT(
            7,
            restoredState.settings.lightSchedule.startHour);
        TEST_ASSERT_EQUAL_INT(
            30,
            restoredState.settings.lightSchedule.startMinute);
        TEST_ASSERT_EQUAL_INT(
            21,
            restoredState.settings.lightSchedule.endHour);
        TEST_ASSERT_EQUAL_INT(
            45,
            restoredState.settings.lightSchedule.endMinute);
        TEST_ASSERT_TRUE(restoredState.fansScheduleAnchor == 12345);
        TEST_ASSERT_TRUE(restoredState.waterScheduleAnchor == 12345);
    }
}

void testDurationScheduleStaysOnWhenDurationEqualsFrequency()
{
    DurationSchedule schedule = testDurationSchedule();
    schedule.durationMinutes = 30 * 60;

    TEST_ASSERT_TRUE(
        isDurationScheduleOn(schedule, 1000, 1000));

    TEST_ASSERT_TRUE(
        isDurationScheduleOn(
            schedule,
            1000,
            1000 + 30 * 60 - 1));

    TEST_ASSERT_TRUE(
        isDurationScheduleOn(
            schedule,
            1000,
            1000 + 30 * 60));
}

void testDurationScheduleStaysOnWhenDurationExceedsFrequency()
{
    DurationSchedule schedule = testDurationSchedule();
    schedule.durationMinutes = (30 * 60) + 1;

    TEST_ASSERT_TRUE(
        isDurationScheduleOn(
            schedule,
            1000,
            1000 + 30 * 60));

    TEST_ASSERT_TRUE(
        isDurationScheduleOn(
            schedule,
            1000,
            1000 + 60 * 60));
}

void testDurationScheduleRejectsZeroAnchor()
{
    DurationSchedule schedule = testDurationSchedule();

    TEST_ASSERT_FALSE(
        isDurationScheduleOn(schedule, 0, 1000));
}

void testDurationScheduleRejectsNegativeAnchor()
{
    DurationSchedule schedule = testDurationSchedule();

    TEST_ASSERT_FALSE(
        isDurationScheduleOn(schedule, -1, 1000));
}

void testDurationScheduleDoesNotTurnOnBeforeAnchor()
{
    DurationSchedule schedule = testDurationSchedule();

    TEST_ASSERT_FALSE(
        isDurationScheduleOn(schedule, 1000, 999));

    TEST_ASSERT_FALSE(
        isDurationScheduleOn(schedule, 1000, 0));
}

void testScheduleControllerUpdateBeforeBeginDoesNothing()
{
    clearSchedulePreferences();

    SystemState state;

    state.fansOn = true;
    state.watering = true;
    state.fansScheduleAnchor = 1234;
    state.waterScheduleAnchor = 5678;

    ScheduleController controller;

    controller.update(state, 10000);

    TEST_ASSERT_TRUE(state.fansOn);
    TEST_ASSERT_TRUE(state.watering);
    TEST_ASSERT_EQUAL_INT(
        1234,
        state.fansScheduleAnchor);
    TEST_ASSERT_EQUAL_INT(
        5678,
        state.waterScheduleAnchor);
}

void testScheduleControllerSetsAutomaticWaterState()
{
    clearSchedulePreferences();

    SystemState state;

    state.settings.modeAuto = true;
    state.settings.waterSchedule = testDurationSchedule();

    ScheduleController controller;

    controller.begin(state);
    controller.update(state, 1000);

    TEST_ASSERT_TRUE(state.watering);

    controller.update(
        state,
        1000 + 10 * 60);

    TEST_ASSERT_FALSE(state.watering);
}

void testScheduleControllerDoesNotResolveOutputsInManualMode()
{
    clearSchedulePreferences();

    SystemState state;

    state.settings.modeAuto = false;

    state.fansOn = true;
    state.watering = true;
    state.lightsOn = true;

    state.settings.fansSchedule = testDurationSchedule();
    state.settings.waterSchedule = testDurationSchedule();

    ScheduleController controller;

    controller.begin(state);
    controller.update(state, 1000 + 10 * 60);

    TEST_ASSERT_TRUE(state.fansOn);
    TEST_ASSERT_TRUE(state.watering);
    TEST_ASSERT_TRUE(state.lightsOn);
}

void testScheduleControllerWaterScheduleChangeResetsOnlyWaterAnchor()
{
    clearSchedulePreferences();

    SystemState state;

    ScheduleController controller;

    controller.begin(state);
    controller.update(state, 1000);

    state.settings.waterSchedule.durationMinutes++;

    controller.update(state, 2000);

    TEST_ASSERT_EQUAL_INT(
        1000,
        state.fansScheduleAnchor);

    TEST_ASSERT_EQUAL_INT(
        2000,
        state.waterScheduleAnchor);
}

void testScheduleControllerLightScheduleChangeDoesNotResetAnchors()
{
    clearSchedulePreferences();

    SystemState state;

    ScheduleController controller;

    controller.begin(state);
    controller.update(state, 1000);

    state.settings.lightSchedule.startHour++;

    controller.update(state, 2000);

    TEST_ASSERT_EQUAL_INT(
        1000,
        state.fansScheduleAnchor);

    TEST_ASSERT_EQUAL_INT(
        1000,
        state.waterScheduleAnchor);
}

void testScheduleControllerRestoresWaterScheduleAfterReboot()
{
    clearSchedulePreferences();

    SystemState savedState;

    savedState.settings.waterSchedule.durationMinutes = 17;
    savedState.settings.waterSchedule.frequencyHours = 42;

    {
        ScheduleController controller;

        controller.begin(savedState);
        controller.update(savedState, 5000);
    }

    SystemState restoredState;

    {
        ScheduleController controller;

        controller.begin(restoredState);

        TEST_ASSERT_EQUAL_INT(
            17,
            restoredState.settings.waterSchedule.durationMinutes);

        TEST_ASSERT_EQUAL_INT(
            42,
            restoredState.settings.waterSchedule.frequencyHours);
    }
}

void testScheduleControllerUsesExistingDefaultsWhenPreferencesAreMissing()
{
    clearSchedulePreferences();

    SystemState state;

    state.settings.lightSchedule.startHour = 9;
    state.settings.lightSchedule.startMinute = 15;
    state.settings.lightSchedule.endHour = 20;
    state.settings.lightSchedule.endMinute = 45;

    state.settings.fansSchedule.durationMinutes = 7;
    state.settings.fansSchedule.frequencyHours = 12;

    state.settings.waterSchedule.durationMinutes = 3;
    state.settings.waterSchedule.frequencyHours = 24;

    ScheduleController controller;

    controller.begin(state);

    TEST_ASSERT_EQUAL_INT(
        9,
        state.settings.lightSchedule.startHour);

    TEST_ASSERT_EQUAL_INT(
        15,
        state.settings.lightSchedule.startMinute);

    TEST_ASSERT_EQUAL_INT(
        20,
        state.settings.lightSchedule.endHour);

    TEST_ASSERT_EQUAL_INT(
        45,
        state.settings.lightSchedule.endMinute);

    TEST_ASSERT_EQUAL_INT(
        7,
        state.settings.fansSchedule.durationMinutes);

    TEST_ASSERT_EQUAL_INT(
        12,
        state.settings.fansSchedule.frequencyHours);

    TEST_ASSERT_EQUAL_INT(
        3,
        state.settings.waterSchedule.durationMinutes);

    TEST_ASSERT_EQUAL_INT(
        24,
        state.settings.waterSchedule.frequencyHours);
}

void testScheduleControllerMissingAnchorsAreInitializedOnUpdate()
{
    clearSchedulePreferences();

    SystemState state;

    state.fansScheduleAnchor = 0;
    state.waterScheduleAnchor = 0;

    ScheduleController controller;

    controller.begin(state);
    controller.update(state, 5000);

    TEST_ASSERT_EQUAL_INT(
        5000,
        state.fansScheduleAnchor);

    TEST_ASSERT_EQUAL_INT(
        5000,
        state.waterScheduleAnchor);
}

void testLightsRemainOffOneMinuteBeforeScheduleStart()
{
    LightSchedule schedule = testSchedule();

    TEST_ASSERT_FALSE(
        isLightsScheduledOn(
            schedule,
            makeTime(7, 59)));
}

void testLightsTurnOnOneMinuteAfterScheduleStart()
{
    LightSchedule schedule = testSchedule();

    TEST_ASSERT_TRUE(
        isLightsScheduledOn(
            schedule,
            makeTime(8, 1)));
}

void testLightsRemainOnOneMinuteBeforeScheduleEnd()
{
    LightSchedule schedule = testSchedule();

    TEST_ASSERT_TRUE(
        isLightsScheduledOn(
            schedule,
            makeTime(21, 59)));
}

void testLightsStayOffExactlyAtScheduleEnd()
{
    LightSchedule schedule = testSchedule();

    TEST_ASSERT_FALSE(
        isLightsScheduledOn(
            schedule,
            makeTime(22, 0)));
}

void setup()
{
    delay(2000);

    UNITY_BEGIN();

    // Lights
    RUN_TEST(testLightsAreOffBeforeSchedule);
    RUN_TEST(testLightsTurnOnAtScheduleStart);
    RUN_TEST(testLightsRemainOnDuringSchedule);
    RUN_TEST(testLightsTurnOffAtScheduleEnd);
    RUN_TEST(testLightsRemainOffOneMinuteBeforeScheduleStart);
    RUN_TEST(testLightsTurnOnOneMinuteAfterScheduleStart);
    RUN_TEST(testLightsRemainOnOneMinuteBeforeScheduleEnd);
    RUN_TEST(testLightsStayOffExactlyAtScheduleEnd);

    RUN_TEST(testManualLightsIgnoreScheduleWhenOff);
    RUN_TEST(testManualLightsIgnoreScheduleWhenOn);
    RUN_TEST(testAutoLightsUseSchedule);

    // Duration schedules
    RUN_TEST(testDurationScheduleIsOffBeforeAnchor);
    RUN_TEST(testDurationScheduleIsOnAtAnchor);
    RUN_TEST(testDurationScheduleTurnsOffAfterDuration);
    RUN_TEST(testDurationScheduleRepeatsAfterFrequency);
    RUN_TEST(testDurationScheduleRejectsInvalidValues);

    RUN_TEST(testDurationScheduleStaysOnWhenDurationEqualsFrequency);
    RUN_TEST(testDurationScheduleStaysOnWhenDurationExceedsFrequency);
    RUN_TEST(testDurationScheduleRejectsZeroAnchor);
    RUN_TEST(testDurationScheduleRejectsNegativeAnchor);
    RUN_TEST(testDurationScheduleDoesNotTurnOnBeforeAnchor);

    // Schedule controller
    RUN_TEST(testScheduleControllerUpdateBeforeBeginDoesNothing);
    RUN_TEST(testScheduleControllerSetsAutomaticFansState);
    RUN_TEST(testScheduleControllerSetsAutomaticWaterState);
    RUN_TEST(testScheduleControllerDoesNotResolveOutputsInManualMode);

    RUN_TEST(testScheduleControllerInitializesMissingAnchors);
    RUN_TEST(testScheduleControllerMissingAnchorsAreInitializedOnUpdate);

    RUN_TEST(testScheduleControllerResetsOnlyChangedScheduleAnchor);
    RUN_TEST(testScheduleControllerWaterScheduleChangeResetsOnlyWaterAnchor);
    RUN_TEST(testScheduleControllerLightScheduleChangeDoesNotResetAnchors);

    RUN_TEST(testScheduleControllerUsesExistingDefaultsWhenPreferencesAreMissing);
    RUN_TEST(testScheduleControllerRestoresWaterScheduleAfterReboot);
    RUN_TEST(testScheduleControllerRestoresStateAfterReboot);

    UNITY_END();
}

void loop()
{
}
