#include "ScheduleController.h"
#include "Lights.h"

namespace
{
    bool schedulesEqual(
        const LightSchedule &left,
        const LightSchedule &right)
    {
        return left.startHour == right.startHour &&
               left.startMinute == right.startMinute &&
               left.endHour == right.endHour &&
               left.endMinute == right.endMinute;
    }

    constexpr time_t SECONDS_PER_MINUTE = 60;
    constexpr time_t SECONDS_PER_HOUR = 60 * SECONDS_PER_MINUTE;

    bool schedulesEqual(
        const DurationSchedule &left,
        const DurationSchedule &right)
    {
        return left.durationMinutes == right.durationMinutes &&
               left.frequencyHours == right.frequencyHours;
    }
}

bool isDurationScheduleOn(
    const DurationSchedule &schedule,
    time_t anchor,
    time_t now)
{
    if (anchor <= 0 || now < anchor ||
        schedule.frequencyHours <= 0 ||
        schedule.durationMinutes <= 0)
    {
        return false;
    }

    time_t period =
        schedule.frequencyHours * SECONDS_PER_HOUR;
    time_t duration =
        schedule.durationMinutes * SECONDS_PER_MINUTE;
    time_t elapsed = now - anchor;

    if (duration >= period)
        return true;

    return (elapsed % period) < duration;
}

void ScheduleController::begin(SystemState &state, const char *preferencesNamespace)
{
    // Open persistent storage and restore the last saved schedules and anchors.
    // A missing anchor is stored as zero and is initialized after valid time is available.
    preferences.begin(preferencesNamespace, false);

    // Light schedule
    state.settings.lightSchedule.startHour =
        preferences.getInt(
            "l_start_hour",
            state.settings.lightSchedule.startHour);
    state.settings.lightSchedule.startMinute =
        preferences.getInt(
            "l_start_min",
            state.settings.lightSchedule.startMinute);
    state.settings.lightSchedule.endHour =
        preferences.getInt(
            "l_end_hour",
            state.settings.lightSchedule.endHour);
    state.settings.lightSchedule.endMinute =
        preferences.getInt(
            "l_end_min",
            state.settings.lightSchedule.endMinute);
    // fans schedule
    state.settings.fansSchedule.durationMinutes =
        preferences.getInt(
            "fan_duration",
            state.settings.fansSchedule.durationMinutes);
    state.settings.fansSchedule.frequencyHours =
        preferences.getInt(
            "fan_frequency",
            state.settings.fansSchedule.frequencyHours);

    // water schedule
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

    savedLightSchedule = state.settings.lightSchedule;
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

    bool changed = updateSchedules(state, now);

    resolveAutomaticOutputs(state, now);

    // Persist the schedules and anchors only when something changed.
    if (changed)
        save(state);
}

bool ScheduleController::updateSchedules(
    SystemState &state,
    time_t now)
{
    bool changed = false;

    // Initialize missing anchors, then reset anchors for edited schedules.
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

    // Save light schedule edits so they survive a reboot.
    if (!schedulesEqual(
            state.settings.lightSchedule,
            savedLightSchedule))
    {
        savedLightSchedule = state.settings.lightSchedule;
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

    return changed;
}

void ScheduleController::resolveAutomaticOutputs(
    SystemState &state,
    time_t now)
{
    if (!state.settings.modeAuto)
        return;

    // Resolve all automatic outputs in one place so App does not need to know
    // how any individual schedule is calculated.
    struct tm localTime;
    localtime_r(&now, &localTime);

    state.lightsOn =
        resolveLightsOn(state.settings, localTime);
    state.fansOn = isDurationScheduleOn(
        state.settings.fansSchedule,
        state.fansScheduleAnchor,
        now);
    state.watering = isDurationScheduleOn(
        state.settings.waterSchedule,
        state.waterScheduleAnchor,
        now);
    state.nextWatering = getNextWateringTime(
        state.settings.waterSchedule,
        state.waterScheduleAnchor,
        now);
}

void ScheduleController::save(SystemState &state)
{
    // Preferences survives reboot, so recurring schedules keep their original
    // absolute anchor instead of starting over after every restart.
    preferences.putInt(
        "l_start_hour",
        state.settings.lightSchedule.startHour);
    preferences.putInt(
        "l_start_min",
        state.settings.lightSchedule.startMinute);
    preferences.putInt(
        "l_end_hour",
        state.settings.lightSchedule.endHour);
    preferences.putInt(
        "l_end_min",
        state.settings.lightSchedule.endMinute);
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

time_t ScheduleController::getNextWateringTime(
    const DurationSchedule &schedule,
    time_t anchor,
    time_t now)
{
    if (anchor <= 0 ||
        schedule.frequencyHours <= 0 ||
        schedule.durationMinutes <= 0)
    {
        return 0;
    }

    time_t period =
        schedule.frequencyHours * SECONDS_PER_HOUR;

    if (now < anchor)
        return anchor;

    time_t elapsed = now - anchor;

    time_t periodsElapsed =
        elapsed / period;

    time_t next =
        anchor + (periodsElapsed + 1) * period;

    // If we're currently inside a watering period,
    // the next watering is the next period.
    return next;
}