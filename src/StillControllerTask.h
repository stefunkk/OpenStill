#ifndef __STILLCONTROLLERTASK_H__
#define __STILLCONTROLLERTASK_H__
#include "Settings.h"
#include "SensorData.h"
#include "StillDataContext.h"
#include <TaskManagerIO.h>

class StillControllerTaskClass: public BaseEvent
{
 public:
	StillControllerTaskClass(SensorDataClass& sensorData, SettingsClass& settings, StillDataContextClass& context);
	void exec() override;
	uint32_t timeOfNextCheck() override;
 private:
	 SensorDataClass& _sensorData;
	 SettingsClass& _settings;
     StillDataContextClass& _context;

     bool isTemperatureLimitReached();
};

#endif // __STILLCONTROLLERTASK_H__