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

const char Names[4][5]{
	"10P",
	"Gora",
	"Keg",
	"Woda"
};

const DeviceAddress Adresses[4]
{
	{ 0x28, 0xFF, 0xB4, 0xCE, 0x70, 0x17, 0x03, 0xBB },
	{ 0x28, 0xFF, 0x4C, 0xDE, 0x83, 0x17, 0x04, 0xF1 },
	{ 0x28, 0xFF, 0xE7, 0x7A, 0x83, 0x17, 0x04, 0x15 },
	{ 0x28, 0xFF, 0xB7, 0x78, 0x90, 0x17, 0x05, 0x77 }
};

auto server = new AsyncWebServer(80);


void setup()
{
	Serial.begin(115200);

	auto* settings = new SettingsClass();
	settings->wifiSsid = WifiSsid;
	settings->wifiPassword = WifiPassword;
	settings->relayPin = SsrPin;
	settings->powerResolution = powerResolutions;
	settings->percentagePower = powerPercentage;
	settings->percentagePower = powerPercentage;
	settings->heaterTimeFrameInSeconds = heaterTimeFrameInSeconds;

	static auto* heaterTask = new HeaterTaskClass(*settings);
	taskManager.registerEvent(heaterTask);

	auto* sensorData = new SensorDataClass;

	auto* oneWire = new OneWire(sensorPin);
	auto* sensors = new DallasTemperature(oneWire);
	static auto* sensorTask = new SensorTaskClass(*sensors, Adresses, *sensorData);
	taskManager.registerEvent(sensorTask);
	
	auto* alcoholCalculator = new AlcoholCalculatorClass;
	auto* lcdService = new LcdServiceClass( *alcoholCalculator, Names, tankSize);
	static auto* lcdTask = new LcdTaskClass(*sensorData, *lcdService);
	taskManager.registerEvent(lcdTask);
	new WifiServerClass(WiFi, *server, *settings, *sensorData);
}

void loop()
{
	taskManager.runLoop();
}
