// HeaterTask.h
#include <TaskManagerIO.h>

#ifndef _HEATERTASK_h
#define _HEATERTASK_h

#include "Settings.h"

class HeaterTaskClass : public BaseEvent
{
public:
	HeaterTaskClass(SettingsClass &settings);
	void exec() override;
	uint32_t timeOfNextCheck() override;
protected:
	SettingsClass& _settingsClass;

	void turnAndWait(int timeInMiliseconds, int state);

	int getHighTime();
	int getLowTime();

	double _waitTime = 1000;
	int _lastState = LOW;
};

#endif
