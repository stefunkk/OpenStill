// SensorTask.h
#ifndef _SCALETASK_h
#define _SCALETASK_h

#include <TaskManagerIO.h>
#include "HX711.h"

#include "StillDataContext.h"
#include "Settings.h"

class ScaleTaskClass : public BaseEvent
{
public:
	ScaleTaskClass(StillDataContextClass &context, HX711 &scale, SettingsClass &settings);
	void exec() override;
	uint32_t timeOfNextCheck() override;
private:
	StillDataContextClass& _context;
	HX711& _scale;
	SettingsClass &_settings;

	unsigned long lastWeightReadingTime = 0;
	double lastWeightReading = 0;
	unsigned long flowRateDelay = 10000;

	void FindSensors();
	void CalculateFlowRate(double weight, unsigned long readingTime);
};

#endif
