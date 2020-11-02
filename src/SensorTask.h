// SensorTask.h
#ifndef _SENSORTASK_h
#define _SENSORTASK_h

#include <DallasTemperature.h>
#include <TaskManagerIO.h>
#include "SensorData.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class SensorTaskClass : public BaseEvent
{
public:
	SensorTaskClass(int sensorPin, DallasTemperature& sensors, DeviceAddress (&addresses)[4], SensorDataClass& sensorData);
	void exec() override;
	uint32_t timeOfNextCheck() override;
private:
	int _sensorPin;
	DallasTemperature& _sensors;
	int _deviceCount;
	DeviceAddress (&_addresses)[4];
	SensorDataClass& _sensorData;
};

extern SensorTaskClass SensorTask;

#endif
