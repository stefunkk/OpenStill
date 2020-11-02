#include "WifiServerTask.h"
#include "LcdTask.h"
#include "SensorData.h"
#include "AlcoholCalculator.h"
#include "LcdService.h"
#include "SensorTask.h"
#include "HeaterTask.h"

#include <TaskManagerIO.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include <Arduino.h>

#include <ESP8266WiFi.h>

#define TANK_SIZE 50

#define LCD_ROWS 4
#define LCD_COLUMNS 20
#define LCD_ADDRESS 0x3F

#define RELAY_PIN D4
#define POWER_RESOLUTION 100
#define POWER_PERCENTAGE 90
#define HEATER_TIME_FRAME_IN_SECONDS 2

#define SENSOR_PIN D7



char Names[4][5]{
	"10P",
	"Gora",
	"Keg",
	"Woda"
};

DeviceAddress Adresses[4]
{
	{ 0x28, 0xFF, 0xB4, 0xCE, 0x70, 0x17, 0x03, 0xBB },
	{ 0x28, 0xFF, 0x4C, 0xDE, 0x83, 0x17, 0x04, 0xF1 },
	{ 0x28, 0xFF, 0xE7, 0x7A, 0x83, 0x17, 0x04, 0x15 },
	{ 0x28, 0xFF, 0xB7, 0x78, 0x90, 0x17, 0x05, 0x77 }
};

WiFiServer server(80);

void setup()
{
	Serial.begin(115200);




	auto* alcoholCalculator = new AlcoholCalculatorClass;

	auto* lcdService = new LcdServiceClass( *alcoholCalculator, Names, TANK_SIZE);



	auto* sensorData = new SensorDataClass;
	
	static auto* heaterTask = new HeaterTaskClass(RELAY_PIN, POWER_RESOLUTION,
		POWER_PERCENTAGE, HEATER_TIME_FRAME_IN_SECONDS);
	taskManager.registerEvent(heaterTask);


	auto* oneWire = new OneWire(SENSOR_PIN);
	auto* sensors = new DallasTemperature(oneWire);
	static auto* sensorTask = new SensorTaskClass(SENSOR_PIN, *sensors, Adresses, *sensorData);
	taskManager.registerEvent(sensorTask);
	
	static auto* lcdTask = new LcdTaskClass(*sensorData, *lcdService);
	taskManager.registerEvent(lcdTask);

	static auto* wifiTask = new WifiServerTaskClass(WiFi, server);
	taskManager.registerEvent(wifiTask);

}

void loop()
{
	taskManager.runLoop();
}
