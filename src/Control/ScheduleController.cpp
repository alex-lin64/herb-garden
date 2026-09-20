#include "ScheduleController.h"

namespace
{
    bool schedulesEqual(
        const DurationSchedule &left,
        const DurationSchedule &right)
    {
        return left.durationMinutes == right.durationMinutes &&
               left.frequencyHours == right.frequencyHours;
    }
}

void ScheduleController::begin(SystemState &state)
{
    // Open persistent storage and restore the last saved schedules and anchors.
    // A missing anchor is stored as zero and is initialized after valid time is available.
    preferences.begin("schedules", false);

    state.settings.fansSchedule.durationMinutes =
        preferences.getInt(
            "fan_duration",
            state.settings.fansSchedule.durationMinutes);
    state.settings.fansSchedule.frequencyHours =
        preferences.getInt(
            "fan_frequency",
            state.settings.fansSchedule.frequencyHours);
    state.settings.waterSchedule.durationMinutes =
        preferences.getInt(
            "water_duration",
            state.settings.waterSchedule.durationMinutes);
    state.settings.waterSchedule.frequencyHours =
        preferences.getInt(
            "water_frequency",
            state.settings.waterSchedule.frequencyHours);

    state.fansScheduleAnchor =
        preferences.getLong64("fan_anchor", 0);
    state.waterScheduleAnchor =
        preferences.getLong64("water_anchor", 0);

    savedFansSchedule = state.settings.fansSchedule;
    savedWaterSchedule = state.settings.waterSchedule;
    stateLoaded = true;
}

void ScheduleController::update(
    SystemState &state,
    time_t now)
{
    if (!stateLoaded)
        return;

    bool changed = false;

    // Initialize anchors for schedules that have never been saved before.
    if (state.fansScheduleAnchor <= 0)
    {
        state.fansScheduleAnchor = now;
        changed = true;
    }

    if (state.waterScheduleAnchor <= 0)
    {
        state.waterScheduleAnchor = now;
        changed = true;
    }

    // Reset only the fans anchor when the fans schedule changes.
    if (!schedulesEqual(
            state.settings.fansSchedule,
            savedFansSchedule))
    {
        savedFansSchedule = state.settings.fansSchedule;
        state.fansScheduleAnchor = now;
        changed = true;
    }

    // Reset only the water anchor when the water schedule changes.
    if (!schedulesEqual(
            state.settings.waterSchedule,
            savedWaterSchedule))
    {
        savedWaterSchedule = state.settings.waterSchedule;
        state.waterScheduleAnchor = now;
        changed = true;
    }

    // Persist the schedules and anchors only when something changed.
    if (changed)
        save(state);
}

void ScheduleController::save(SystemState &state)
{
    // Preferences survives reboot, so recurring schedules keep their original
    // absolute anchor instead of starting over after every restart.
    preferences.putInt(
        "fan_duration",
        state.settings.fansSchedule.durationMinutes);
    preferences.putInt(
        "fan_frequency",
        state.settings.fansSchedule.frequencyHours);
    preferences.putInt(
        "water_duration",
        state.settings.waterSchedule.durationMinutes);
    preferences.putInt(
        "water_frequency",
        state.settings.waterSchedule.frequencyHours);
    preferences.putLong64(
        "fan_anchor",
        state.fansScheduleAnchor);
    preferences.putLong64(
        "water_anchor",
        state.waterScheduleAnchor);
}
