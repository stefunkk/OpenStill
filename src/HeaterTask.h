// HeaterTask.h
#include <TaskManagerIO.h>

#ifndef _HEATERTASK_h
#define _HEATERTASK_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class HeaterTaskClass : public BaseEvent
{
public:
	HeaterTaskClass(int relayPin, int powerResolution, int percentagePower, int heaterTimeFrameInSeconds);
	void exec() override;
	uint32_t timeOfNextCheck() override;
	void UpdateHeaterParameters(int powerResolution, int percentagePower, int heaterTimeFrameInSeconds);
protected:
	int _relayPin = 0,
	_powerResolution = 0,
	_percentagePower = 0,
	_heaterTimeFrameInSeconds = 0,
	_highTimeInMiliseconds = 0,
	_lowTimeInMiliseconds = 0;
	void turnAndWait(int timeInMiliseconds, int state);

	double _waitTime = 1000;
	int _lastState = LOW;
};

extern HeaterTaskClass HeaterTask;

#endif
