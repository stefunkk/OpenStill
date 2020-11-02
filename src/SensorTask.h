// SensorTask.h
#ifndef _SENSORTASK_h
#define _SENSORTASK_h

#include <DallasTemperature.h>
#include <TaskManagerIO.h>
#include "SensorData.h"

class SensorTaskClass : public BaseEvent
{
public:
	SensorTaskClass(DallasTemperature& sensors, const DeviceAddress (&addresses)[4], SensorDataClass& sensorData);
	void exec() override;
	uint32_t timeOfNextCheck() override;
private:
	DallasTemperature& _sensors;
	int _deviceCount;
	const DeviceAddress (&_addresses)[4];
	SensorDataClass& _sensorData;
};

#endif
