#pragma once

#include <Preferences.h>
#include <time.h>
#include "../State/State.h"

bool isDurationScheduleOn(
    const DurationSchedule &schedule,
    time_t anchor,
    time_t now);

class ScheduleController
{
public:
    void begin(SystemState &state, const char *preferencesNamespace = "schedules");
    void update(SystemState &state, time_t now);

private:
    Preferences preferences;
    LightSchedule savedLightSchedule;
    DurationSchedule savedFansSchedule;
    DurationSchedule savedWaterSchedule;
    bool stateLoaded = false;

    void save(SystemState &state);
    bool updateSchedules(SystemState &state, time_t now);
    void resolveAutomaticOutputs(SystemState &state, time_t now);
};
