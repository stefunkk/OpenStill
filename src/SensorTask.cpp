#include "SensorTask.h"

SensorTaskClass::SensorTaskClass(OneWire &oneWire, DallasTemperature &sensors, StillDataContextClass &context, SensorDataClass &sensorData) : _sensors(sensors), _sensorData(sensorData), _oneWire(oneWire), _context(context)
{
	_sensors.begin();

	_deviceCount = _sensors.getDeviceCount();
	FindSensors();
	_sensors.setResolution(12);
}

uint32_t SensorTaskClass::timeOfNextCheck()
{
	setTriggered(true);

	return millisToMicros(1500);
}

void SensorTaskClass::FindSensors()
{
	DeviceAddress addr;

	int counter = 0;
	while (_oneWire.search(addr))
	{
		if (OneWire::crc8(addr, 7) != addr[7])
		{
			Serial.print("CRC is not valid!\n");
			return;
		}

		char deviceAddressName[100];
		sprintf(deviceAddressName, "0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x", 
			addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], addr[6], addr[7]);
		
		_context.foundDeviceString[counter] = deviceAddressName;
		
		for (int i = 0; i < 8; i++)
		{
			_context.foundDevices[counter][i] = addr[i];
		}

		counter++;
	}

	_oneWire.reset_search();
	_context.findSensors = false;
	return;
}

void SensorTaskClass::exec()
{
	if (_context.findSensors)
	{
		FindSensors();
	}

	_sensors.requestTemperatures();

	double reading = -127;
	reading = _sensors.getTempC(_context.shelf10Address);

	if (reading != -127) {

		_sensorData.shelf10 = reading;
	}

	reading = _sensors.getTempC(_context.headAddress);

	if (reading != -127) {

		_sensorData.header = reading;
	}

	reading = _sensors.getTempC(_context.tankAddress);

	if (reading != -127) {

		_sensorData.tank = reading;
	}
	
	reading = _sensors.getTempC(_context.waterAddress);

	if (reading != -127) {

		_sensorData.water = reading;
	}
}

