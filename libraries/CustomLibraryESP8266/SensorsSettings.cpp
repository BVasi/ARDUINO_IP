#include <SensorsSettings.h>

SensorsSettings::SensorsSettings()
{
    sensorsReferences_ = SensorsReferences();
    samplingPeriod_ = SamplingPeriod::HALF_MINUTE;
}

void SensorsSettings::updateSamplingPeriod(const String& newSamplingPeriodString)
{
    if (newSamplingPeriodString == "HALF_MINUTE")
    {
        samplingPeriod_ = SamplingPeriod::HALF_MINUTE;
        return;
    }
    if (newSamplingPeriodString == "ONE_MINUTE")
    {
        samplingPeriod_ = SamplingPeriod::ONE_MINUTE;
        return;
    }
    if (newSamplingPeriodString == "THREE_MINUTES")
    {
        samplingPeriod_ = SamplingPeriod::THREE_MINUTES;
        return;
    }
    if (newSamplingPeriodString == "FIVE_MINUTES")
    {
        samplingPeriod_ = SamplingPeriod::FIVE_MINUTES;
        return;
    }
    if (newSamplingPeriodString == "TEN_MINUTES")
    {
        samplingPeriod_ = SamplingPeriod::TEN_MINUTES;
        return;
    }
    samplingPeriod_ = SamplingPeriod::FIVE_MINUTES;
}

void SensorsSettings::updateSensorSettings(const JsonDocument& newSamplingPeriodString)
{
  updateSamplingPeriod(newSamplingPeriodString["newSensorSettings"]["samplingPeriod"]);
  sensorsReferences_.maximumBloodPressure_ = newSamplingPeriodString["newSensorSettings"]["sensorsReferences"]["maximumBloodPressure"];
  sensorsReferences_.maximumBodyTemperature_ = newSamplingPeriodString["newSensorSettings"]["sensorsReferences"]["maximumBodyTemperature"];
  sensorsReferences_.maximumGlucose_ = newSamplingPeriodString["newSensorSettings"]["sensorsReferences"]["maximumGlucose"];
  sensorsReferences_.maximumPulse_ = newSamplingPeriodString["newSensorSettings"]["sensorsReferences"]["maximumPulse"];
  sensorsReferences_.maximumRoomTemperature_ = newSamplingPeriodString["newSensorSettings"]["sensorsReferences"]["maximumRoomTemperature"];
  sensorsReferences_.maximumWeight_ = newSamplingPeriodString["newSensorSettings"]["sensorsReferences"]["maximumWeight"];
  sensorsReferences_.minimumBloodPressure_ = newSamplingPeriodString["newSensorSettings"]["sensorsReferences"]["minimumBloodPressure"];
  sensorsReferences_.minimumBodyTemperature_ = newSamplingPeriodString["newSensorSettings"]["sensorsReferences"]["minimumBodyTemperature"];
  sensorsReferences_.minimumGlucose_ = newSamplingPeriodString["newSensorSettings"]["sensorsReferences"]["minimumGlucose"];
  sensorsReferences_.minimumPulse_ = newSamplingPeriodString["newSensorSettings"]["sensorsReferences"]["minimumPulse"];
  sensorsReferences_.minimumRoomTemperature_ = newSamplingPeriodString["newSensorSettings"]["sensorsReferences"]["minimumRoomTemperature"];
  sensorsReferences_.minimumWeight_ = newSamplingPeriodString["newSensorSettings"]["sensorsReferences"]["minimumWeight"];
}

AlarmType SensorsSettings::shouldRaiseAlarm(const SensorsData& sensorsData)
{
    return sensorsReferences_.shouldRaiseAlarm(sensorsData);
}

int SensorsSettings::getSamplingPeriodInSeconds() const
{
    return samplingPeriod_;
}