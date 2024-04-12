#pragma once
#include <SensorsReferences.h>

enum SamplingPeriod
{
    HALF_MINUTE = 30,
    ONE_MINUTE = 60,
    THREE_MINUTES = 180,
    FIVE_MINUTES = 300,
    TEN_MINUTES = 600
};

struct SensorsSettings
{
    SensorsSettings();

    SensorsReferences sensorsReferences_;
    SamplingPeriod samplingPeriod_;
};