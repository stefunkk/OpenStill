// WifiServerTask.h

#ifndef _WIFISERVERTASK_h
#define _WIFISERVERTASK_h

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include "Settings.h"
#include "SensorData.h"
#include "StillDataContext.h"
#include "ConfigurationService.h"
#include "AlcoholCalculator.h"
#include "NotificationHelper.h"

class WifiServerClass
{
public:
	WifiServerClass(ESP8266WiFiClass &wifi, AsyncWebServer &server, SettingsClass &wifiSettings, SensorDataClass &_sensorData,
					StillDataContextClass &context, ConfigurationServiceClass &configurationService);
	void connectToWifi();


private:
	void configurePages();
	void configureInputs();

	const char *_heater = "heater";
	const char *_tankSize = "tankSize";
	const char *_findSensors = "findSensors";
	const char *_csvTimeFrameInSeconds = "csvTimeFrameInSeconds";

	const char *_pushNotificationCode = "pushNotificationCode";

	const char *_shelf10Device = "shelf10Device";
	const char *_headerDevice = "headerDevice";
	const char *_tankDevice = "tankDevice";
	const char *_waterDevice = "waterDevice";

	const char *_shelf10TemperatureLimit = "shelf10TemperatureLimit";
	const char *_headerTemperatureLimit = "headerTemperatureLimit";
	const char *_tankTemperatureLimit = "tankTemperatureLimit";
	const char *_waterTemperatureLimit = "waterTemperatureLimit";


	ESP8266WiFiClass &_wifi;
	AsyncWebServer &_server;
	SettingsClass &_settings;
	SensorDataClass &_sensorData;
	StillDataContextClass &_context;
	ConfigurationServiceClass &_configurationService;

	void UpdateDeviceAddress(String index, DeviceAddress &address);
};

#endif
