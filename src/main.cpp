#include "ConfigurationService.h"
#include "StillDataContext.h"
#include "WifiServer.h"
#include "LcdTask.h"
#include "SensorData.h"
#include "AlcoholCalculator.h"
#include "LcdService.h"
#include "SensorTask.h"
#include "HeaterTask.h"
#include "Settings.h"
#include <TaskManagerIO.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// DS18B20 sensor settings
const uint8_t sensorPin = D7;

// Heater settings
const uint8_t SsrPin = D4;
const int powerResolutions = 100;
const int powerPercentage = 0;
const int heaterTimeFrameInSeconds = 2;
const int tankSize = 50;

// WIFI Settings
const String WifiSsid = "";
const String WifiPassword = "";

String Shelv10Name = "10P";
String HeadName = "Gora";
String TankName = "Keg";
String WaterName = "Woda";

DeviceAddress Shelv10Address = {0x28, 0xFF, 0xB4, 0xCE, 0x70, 0x17, 0x03, 0xBB};
DeviceAddress HeadAddress = {0x28, 0xFF, 0x4C, 0xDE, 0x83, 0x17, 0x04, 0xF1};
DeviceAddress TankAddress = {0x28, 0xFF, 0xE7, 0x7A, 0x83, 0x17, 0x04, 0x15};
DeviceAddress WaterAddress = {0x28, 0xFF, 0xB7, 0x78, 0x90, 0x17, 0x05, 0x77};

auto server = new AsyncWebServer(80);


void setup()
{

	Serial.begin(115200);
    SPIFFS.begin();

	delay(2000);

	auto *settings = new SettingsClass();
	settings->wifiSsid = WifiSsid;
	settings->wifiPassword = WifiPassword;
	settings->relayPin = SsrPin;
	settings->powerResolution = powerResolutions;
	settings->percentagePower = powerPercentage;
	settings->percentagePower = powerPercentage;
	settings->heaterTimeFrameInSeconds = heaterTimeFrameInSeconds;

	auto *context = new StillDataContextClass(
		Shelv10Address,
		HeadAddress,
		TankAddress,
		WaterAddress,
		Shelv10Name,
		HeadName,
		TankName,
		WaterName);

	auto *configurationService = new ConfigurationServiceClass(*context, *settings);
	configurationService->loadConfiguration();

	static auto *heaterTask = new HeaterTaskClass(*settings);

	auto *sensorData = new SensorDataClass;

	auto *oneWire = new OneWire(sensorPin);
	auto *sensors = new DallasTemperature(oneWire);
	static auto *sensorTask = new SensorTaskClass(*oneWire, *sensors, *context, *sensorData);

	auto *alcoholCalculator = new AlcoholCalculatorClass;
	auto *lcdService = new LcdServiceClass(*alcoholCalculator, *context, tankSize);
	static auto *lcdTask = new LcdTaskClass(*sensorData, *lcdService);
	
	auto* wifiClass = new WifiServerClass(WiFi, *server, *settings, *sensorData, *context, *configurationService);
	wifiClass->connectToWifi();
	
	taskManager.registerEvent(lcdTask);
	taskManager.registerEvent(sensorTask);
	taskManager.registerEvent(heaterTask);

}

void loop()
{
	taskManager.runLoop();
}