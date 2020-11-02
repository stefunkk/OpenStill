// LcdTask.h

#ifndef _LCDTASK_h
#define _LCDTASK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "SensorData.h"
#include "LcdService.h"
#include <TaskManagerIO.h>

class LcdTaskClass: public BaseEvent
{
 public:
	LcdTaskClass(SensorDataClass& sensorData, LcdServiceClass& lcdService);
	void exec() override;
	uint32_t timeOfNextCheck() override;
 private:
	 SensorDataClass& _sensorData;
	 LcdServiceClass& _lcdService;
};

extern LcdTaskClass LcdTask;

#endif

