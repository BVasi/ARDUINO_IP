#pragma once
#include "SensorsReferences.h"

enum SamplingPeriod
{
    HALF_MINUTE = 30,
    ONE_MINUTE = 60,
    THREE_MINUTES = 180,
    FIVE_MINUTES = 300,
    TEN_MINUTES = 600
};

class SensorsSettings
{
    public:
        SensorsSettings();
        void updateSensorSettings(const JsonDocument& newSensorsSettings);
        AlarmType shouldRaiseAlarm(const SensorsData& sensorsData);
        int getSamplingPeriodInSeconds() const;
    private:
        void updateSamplingPeriod(const String& newSamplingPeriodString);

        SensorsReferences sensorsReferences_;
        SamplingPeriod samplingPeriod_;
};