#include <SensorsSettings.h>

SensorsSettings::SensorsSettings()
{
    sensorsReferences_ = SensorsReferences();
    samplingPeriod_ = SamplingPeriod::FIVE_MINUTES;
}