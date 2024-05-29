#pragma once
#include "SensorsData.h"
#include "Alarm.h"

struct SensorsReferences
{
    SensorsReferences();
    AlarmType shouldRaiseAlarm(const SensorsData& sensorsData);

    int minimumBloodPressure_;
    int maximumBloodPressure_;
    int minimumPulse_;
    int maximumPulse_;
    int minimumBodyTemperature_;
    int maximumBodyTemperature_;
    int minimumWeight_;
    int maximumWeight_;
    int minimumGlucose_;
    int maximumGlucose_;
    int minimumRoomTemperature_;
    int maximumRoomTemperature_;
};