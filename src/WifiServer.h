// WifiServerTask.h

#ifndef _WIFISERVERTASK_h
#define _WIFISERVERTASK_h

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include "Settings.h"
#include "SensorData.h"
#include <LittleFS.h>

class WifiServerClass
{
 public:
	 WifiServerClass(ESP8266WiFiClass& wifi, AsyncWebServer& server, SettingsClass& wifiSettings, SensorDataClass& _sensorData);
private:
	void connectToWifi();
	void configureLittleFS();
	void configurePages();
	void configureInputs();

	const char* _heater = "heater";
	const char* _tankSize = "tankSize";
	
	ESP8266WiFiClass& _wifi;
	AsyncWebServer& _server;
	SettingsClass& _settings;
	SensorDataClass& _sensorData;
};

#endif

