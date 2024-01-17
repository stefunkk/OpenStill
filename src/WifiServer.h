// WifiServerTask.h

#ifndef _WIFISERVERTASK_h
#define _WIFISERVERTASK_h

#include <Arduino.h>
#include "Settings.h"
#include "SensorData.h"
#include "StillDataContext.h"
#include "ConfigurationService.h"
#include "AlcoholCalculator.h"
#include "NotificationHelper.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

class WifiServerClass
{
public:
	WifiServerClass(AsyncWebServer &server, SettingsClass &wifiSettings, SensorDataClass &_sensorData,
					StillDataContextClass &context, ConfigurationServiceClass &configurationService);
	void connectToWifi();
	void setupAccessPoint();


private:
	AsyncWebServer &_server;
	SettingsClass &_settings;
	SensorDataClass &_sensorData;
	StillDataContextClass &_context;
	ConfigurationServiceClass &_configurationService;

	const char *_heater = "heater";
	const char *_heater2 = "heater2";
	const char *_heater3 = "heater3";
	const char *_tankSize = "tankSize";
	const char *_findSensors = "findSensors";
	const char *_csvTimeFrameInSeconds = "csvTimeFrameInSeconds";
	const char *_tempOfTheDay = "tempOfTheDay";
	const char *_tempOfTheDayNotificationDelayInSeconds = "tempOfTheDayNotificationDelayInSeconds";
	const char *_tempOfTheDayDeviation = "tempOfTheDayDeviation";
	
	const char *_wifiSsid = "wifiSsid";
	const char *_wifiPassword = "wifiPassword";
	
	const char *_scaleOffset = "scaleOffset";

	const char *_pushNotificationCode = "pushNotificationCode";

	const char *_shelf10Device = "shelf10Device";
	const char *_headerDevice = "headerDevice";
	const char *_tankDevice = "tankDevice";
	const char *_waterDevice = "waterDevice";

	const char *_shelf10TemperatureLimit = "shelf10TemperatureLimit";
	const char *_headerTemperatureLimit = "headerTemperatureLimit";
	const char *_tankTemperatureLimit = "tankTemperatureLimit";
	const char *_waterTemperatureLimit = "waterTemperatureLimit";

	const char *_scaleWeightNotification = "scaleWeightNotification";

	const char *_shelf10TemperatureNotification = "shelf10TemperatureNotification";
	const char *_headerTemperatureNotification = "headerTemperatureNotification";
	const char *_tankTemperatureNotification = "tankTemperatureNotification";
	const char *_waterTemperatureNotification = "waterTemperatureNotification";
	
	void configureAPPages();
	void configurePages();
	void configureInputs();



	void UpdateDeviceAddress(String index, DeviceAddress &address);
};

#endif
