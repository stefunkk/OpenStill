#include "SensorTask.h"

SensorTaskClass::SensorTaskClass(int sensorPin, DallasTemperature& sensors, 
	DeviceAddress (&addresses)[4], SensorDataClass& sensorData):
	_sensorPin(sensorPin), _sensors(sensors), _addresses(addresses), _sensorData(sensorData)
{
	_sensors.setResolution(12);
	_sensors.begin();

	_deviceCount = _sensors.getDeviceCount();
}

uint32_t SensorTaskClass::timeOfNextCheck()
{
	setTriggered(true);

	return millisToMicros(1000);
}

void SensorTaskClass::exec()
{

	Serial.println("Sensor Task");
	
	auto counter = 0;
	_sensors.requestTemperatures();

	_sensorData.shelf10 = _sensors.getTempC(_addresses[0]);
	_sensorData.header = _sensors.getTempC(_addresses[1]);
	_sensorData.tank = _sensors.getTempC(_addresses[2]);
	_sensorData.water = _sensors.getTempC(_addresses[3]);
}

