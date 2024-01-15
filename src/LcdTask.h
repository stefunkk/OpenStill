// LcdTask.h

#ifndef _LCDTASK_h
#define _LCDTASK_h

#include "SensorData.h"
#include "LcdService.h"
#include "StillDataContext.h"
#include <TaskManagerIO.h>

class LcdTaskClass: public BaseEvent
{
 public:
	LcdTaskClass(StillDataContextClass& context, SensorDataClass& sensorData, LcdServiceClass& lcdService);
	void exec() override;
	uint32_t timeOfNextCheck() override;
 private:
	 StillDataContextClass& _context;
	 SensorDataClass& _sensorData;
	 LcdServiceClass& _lcdService;
	 bool _shouldReset;
};

#endif

