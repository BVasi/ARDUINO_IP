#include <Ticker.h>
#include <ArduinoJson.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <PulseSensorPlayground.h>
#include "NetworkWrapper32.h"
#include "SensorsData.h"
#include "SensorsSettings.h"
#include "Adafruit_Si7021.h"

namespace EndPoints
{
  const String REMOTE_SERVER_ADDRESS = "http://ip_real:5000"; //schimbat cu real
  const String SAVE_SENSORS_DATA = REMOTE_SERVER_ADDRESS + "/save_sensors_data";
  const String CHANGE_SENSORS_SETTINGS = "/change_sensors_settings";
  const String REPORT_ALARM = REMOTE_SERVER_ADDRESS + "/report_alarm";
};

namespace SensorObjects
{
  PulseSensorPlayground pulseSensor;
  Adafruit_Si7021 roomTemperatureSensor = Adafruit_Si7021();
};

const int SERVER_PORT = 80;
const int ONE_SECOND = 1000;
const String cnp = "to_be_changed"; // schimbat pentru ce user se doreste aplicatia atunci cand se monteaza esp-ul in casa

NetworkWrapper networkWrapper;
WebServer server(SERVER_PORT);
Ticker sendDataTimer;

SensorsSettings sensorsSettings = SensorsSettings();

void updateSendDataTimer()
{
  sendDataTimer.detach();
  sendDataTimer.attach(sensorsSettings.getSamplingPeriodInSeconds(), sendPostRequest);
}

void handleChangeSensorSettings()
{
  Serial.println("handleChangeSensorSettings()");
  JsonDocument receivedData;
  DeserializationError error = deserializeJson(receivedData, server.arg("plain"));
  if (error)
  {
    server.send(400);
    return;
  }
  sensorsSettings.updateSensorSettings(receivedData);
  updateSendDataTimer();
  server.send(200);
}

AlarmType shouldRaiseAlarm(const SensorsData& sensorsData)
{
  if (sensorsData.isFlooded_)
  {
    return AlarmType::IS_FLOODED;
  }
  if (sensorsData.isGasPresent_)
  {
    return AlarmType::GAS_DETECTED;
  }
  if (sensorsSettings.shouldRaiseAlarm(sensorsData) != AlarmType::NONE)
  {
    return sensorsSettings.shouldRaiseAlarm(sensorsData);
  }
  return AlarmType::NONE;
}

String getAdditionalAlarmText(const SensorsData& sensorsData, const AlarmType& alarmType)
{
  String additionalAlarmText = "";
  switch (alarmType)
  {
    case GAS_DETECTED:
    {
      additionalAlarmText = "Gaz detectat!";
      break;
    }
    case IS_FLOODED:
    {
      additionalAlarmText = "Inundatie detectata!";
      break;
    }
    case TEMPERATURE_INCREASE:
    {
      additionalAlarmText = "Temperatura a depasit " + String(sensorsData.roomTemperature_) + " grade Celsius!";
      break;
    }
    case UNUSUAL_BODY_PARAMETERS:
    {
      additionalAlarmText = "Pulsul a fost " + String(sensorsData.pulse_) + " BPM!";
      break;
    }
  }
  return additionalAlarmText;
}

SensorsData getSensorsData()
{
  SensorsData sensorsData = SensorsData();
  int rawValue = analogRead(SensorPin::LIGHT_SENSOR);
  float voltage = rawValue * (3.3 / 1023) * 1000;
  float resitance = 10000 * (voltage / (5000.0 - voltage));
  if (SensorObjects::pulseSensor.sawStartOfBeat())
  {
    sensorsData.pulse_ = SensorObjects::pulseSensor.getBeatsPerMinute();
  }
  sensorsData.roomTemperature_ = SensorObjects::roomTemperatureSensor.readTemperature();
  sensorsData.isFlooded_ = (65 < SensorObjects::roomTemperatureSensor.readHumidity());
  sensorsData.isInProximity_ = (0 < digitalRead(SensorPin::PROXIMITY_SENSOR));
  sensorsData.isGasPresent_ = (0 < digitalRead(SensorPin::GAS_SENSOR));
  sensorsData.isLightPresent_ = (resitance < 7000);

  return sensorsData;
}

void sendPostRequest()
{
  HTTPClient alarmHttpClient, sensorHttpClient;

  SensorsData sensorsData = getSensorsData();
  AlarmType alarmType = AlarmType::NONE;
  if (((alarmType = shouldRaiseAlarm(sensorsData)) != AlarmType::NONE))
  {
    Alarm alarm = Alarm(getAdditionalAlarmText(sensorsData, alarmType), alarmType);
    alarmHttpClient.begin(EndPoints::REPORT_ALARM);
    alarmHttpClient.addHeader("Content-Type", "application/json");
    alarmHttpClient.POST(alarm.serialize(cnp));
  }
  sensorHttpClient.begin(EndPoints::SAVE_SENSORS_DATA);
  sensorHttpClient.addHeader("Content-Type", "application/json");
  sensorHttpClient.POST(sensorsData.serialize(cnp));
}

void setup()
{
  Serial.begin(115200);
  pinMode(SensorPin::PROXIMITY_SENSOR, INPUT);
  SensorObjects::pulseSensor.analogInput(SensorPin::PULSE_SENSOR);
  SensorObjects::pulseSensor.setThreshold(300);
  if (!SensorObjects::roomTemperatureSensor.begin())
  {
    Serial.println("Did not find SI7021 sensor!");
  }
  if (!SensorObjects::pulseSensor.begin())
  {
    Serial.println("Did not find pulse sensor!");
  }
  String ssid = "SSID_READ"; // schimbat cu datele reale ale userului atunci cand se monteaza in casa
  String password = "PASSWORD_REAL"; // schimbat cu datele reale ale userului atunci cand se monteaza in casa
  networkWrapper.setNetworkDetails(ssid.c_str(), password.c_str());
  networkWrapper.connectToNetwork();
  server.on(EndPoints::CHANGE_SENSORS_SETTINGS + "/" + cnp, HTTP_POST, handleChangeSensorSettings);
  server.begin();
  sendPostRequest();
  sendDataTimer.attach(sensorsSettings.getSamplingPeriodInSeconds(), sendPostRequest);
}

void loop()
{
  if (!networkWrapper.isConnectedToNetwork())
  {
    networkWrapper.connectToNetwork();
  }
  server.handleClient();
  delay(ONE_SECOND);
}