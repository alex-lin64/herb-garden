#pragma once

#include <time.h>
#include "../State/State.h"

bool isLightsScheduledOn(
    const LightSchedule &schedule,
    const struct tm &localTime);

bool resolveLightsOn(
    const SystemSettings &settings,
    const struct tm &localTime);