#pragma once

#include <Preferences.h>
#include <time.h>
#include "../State/State.h"

class ScheduleController
{
public:
    void begin(SystemState &state);
    void update(SystemState &state, time_t now);

private:
    Preferences preferences;
    DurationSchedule savedFansSchedule;
    DurationSchedule savedWaterSchedule;
    bool stateLoaded = false;

    void save(SystemState &state);
};
