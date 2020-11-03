// SensorTask.h
#ifndef _SENSORTASK_h
#define _SENSORTASK_h

#include <DallasTemperature.h>
#include <TaskManagerIO.h>
#include <OneWire.h>
#include "SensorData.h"
#include "StillDataContext.h"

class SensorTaskClass : public BaseEvent
{
public:
	SensorTaskClass(OneWire& oneWire, DallasTemperature& sensors, StillDataContextClass& context, SensorDataClass& sensorData);
	void exec() override;
	uint32_t timeOfNextCheck() override;
private:
	void FindSensors();
	DallasTemperature& _sensors;
	int _deviceCount;
	SensorDataClass& _sensorData;
	OneWire& _oneWire;
	StillDataContextClass& _context;
};

#endif
