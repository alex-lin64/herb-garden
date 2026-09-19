#include "Lights.h"

namespace
{
    constexpr int MINUTES_PER_HOUR = 60;
    constexpr int DAYS_PER_WEEK = 7;
}

bool isLightsScheduledOn(
    const LightSchedule &schedule,
    const struct tm &localTime)
{
    int currentMinutes =
        localTime.tm_hour * MINUTES_PER_HOUR + localTime.tm_min;
    int startMinutes =
        schedule.startHour * MINUTES_PER_HOUR + schedule.startMinute;
    int endMinutes =
        schedule.endHour * MINUTES_PER_HOUR + schedule.endMinute;

    if (schedule.frequencyDays > 1 &&
        (localTime.tm_yday % schedule.frequencyDays) != 0)
    {
        return false;
    }

    return currentMinutes >= startMinutes && currentMinutes < endMinutes;
}

bool resolveLightsOn(
    const SystemSettings &settings,
    const struct tm &localTime)
{
    if (!settings.modeAuto)
        return settings.manualLightsOn;

    return isLightsScheduledOn(settings.lightSchedule, localTime);
}