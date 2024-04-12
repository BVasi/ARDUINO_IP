#include <SensorsReferences.h>

SensorsReferences::SensorsReferences()
{
    minimumBloodPressure_ = 90;
    maximumBloodPressure_ = 120;
    minimumPulse_ = 40;
    maximumPulse_ = 150;
    minimumBodyTemperature_ = 32;
    maximumBodyTemperature_ = 40;
    minimumWeight_ = 40;
    maximumWeight_ = 140;
    minimumGlucose_ = 90;
    maximumGlucose_ = 120;
    minimumRoomTemperature_ = 16;
    maximumRoomTemperature_ = 30;
}

AlarmType SensorsReferences::shouldRaiseAlarm(const SensorsData& sensorsData)
{
    if ((sensorsData.roomTemperature_ > maximumRoomTemperature_))
    {
        return AlarmType::TEMPERATURE_INCREASE;
    }
    if ((sensorsData.pulse_ < minimumPulse_) ||
     (sensorsData.pulse_ > maximumPulse_))
    {
        return AlarmType::UNUSUAL_BODY_PARAMETERS;
    }
    return AlarmType::NONE;
}