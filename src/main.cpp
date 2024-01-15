#include "ConfigurationService.h"
#include "StillDataContext.h"
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
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "FileService.h"
#include "StillDataTask.h"
#include "StillControllerTask.h"
#include "NotificationTask.h"
#include "WifiServer.h"

// // DS18B20 sensor settings
const uint8_t sensorPin = GPIO_NUM_15;
const int csvTimeFrameInSeconds = 30;
const bool saveCsv = true;

// // Heater settings
const uint8_t Heater1 = GPIO_NUM_23;
const uint8_t Heater2 = GPIO_NUM_22;
const uint8_t Heater3 = GPIO_NUM_1;

const int powerResolutions = 100;
const int powerPercentage = 0;
const int heaterTimeFrameInSeconds = 2;
const int tankSize = 50;

// // WIFI Settings
const String WifiSsid = "";
const String WifiPassword = "";

String shelf10Name = "10P";
String HeadName = "Gora";
String TankName = "Keg";
String WaterName = "Woda";

DeviceAddress shelf10Address = {0x28, 0xFF, 0xB4, 0xCE, 0x70, 0x17, 0x03, 0xBB};
DeviceAddress HeadAddress = {0x28, 0xFF, 0x4C, 0xDE, 0x83, 0x17, 0x04, 0xF1};
DeviceAddress TankAddress = {0x28, 0xFF, 0xE7, 0x7A, 0x83, 0x17, 0x04, 0x15};
DeviceAddress WaterAddress = {0x28, 0xFF, 0xB7, 0x78, 0x90, 0x17, 0x05, 0x77};

auto server = new AsyncWebServer(80);

void setup()
{

	Serial.begin(115200);

	auto *settings = new SettingsClass();
	settings->wifiSsid = WifiSsid;
	settings->wifiPassword = WifiPassword;
	settings->relayPin1 = Heater1;
	settings->relayPin2 = Heater2;
	settings->relayPin3 = Heater3;
	settings->powerResolution = powerResolutions;
	settings->percentagePower = powerPercentage;
	settings->percentagePower = powerPercentage;
	settings->heaterTimeFrameInSeconds = heaterTimeFrameInSeconds;
	settings->csvTimeFrameInSeconds = csvTimeFrameInSeconds;
	settings->saveCsv = saveCsv;

	auto *context = new StillDataContextClass(
		shelf10Address,
		HeadAddress,
		TankAddress,
		WaterAddress,
		shelf10Name,
		HeadName,
		TankName,
		WaterName);

	auto *fileService = new FileServiceClass();
	auto *configurationService = new ConfigurationServiceClass(*fileService, *context, *settings);
	configurationService->loadConfiguration();

	static auto *heaterTask = new HeaterTaskClass(*settings);

	auto *sensorData = new SensorDataClass;

	auto *oneWire = new OneWire(sensorPin);
	auto *sensors = new DallasTemperature(oneWire);
	static auto *sensorTask = new SensorTaskClass(*oneWire, *sensors, *context, *sensorData);

	auto *alcoholCalculator = new AlcoholCalculatorClass;
	auto *lcdService = new LcdServiceClass(*alcoholCalculator, *context, tankSize);
	static auto *lcdTask = new LcdTaskClass(*context, *sensorData, *lcdService);

	auto *stillDataTask = new StillDataTaskClass(*context, *fileService, *sensorData, *settings);

	auto *stillControllerTask = new StillControllerTaskClass(*sensorData, *settings, *context);

	auto *notificationTask = new NotificationTaskClass(*context, *settings);

	auto *wifiClass = new WifiServerClass(*server, *settings, *sensorData, *context, *configurationService);

	if (settings->wifiSsid == nullptr || settings->wifiSsid == "" || settings->wifiSsid == "null")
	{
		Serial.print("Setting up access point");
		wifiClass->setupAccessPoint();
	}
	else
	{
		Serial.print("Connecting to WiFi");
		wifiClass->connectToWifi();
	}

	taskManager.registerEvent(lcdTask);
	taskManager.registerEvent(sensorTask);
	taskManager.registerEvent(heaterTask);
	taskManager.registerEvent(stillDataTask);
	taskManager.registerEvent(stillControllerTask);
	taskManager.registerEvent(notificationTask);
}

void loop()
{
	taskManager.runLoop();
}