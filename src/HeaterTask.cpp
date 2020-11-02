#include "HeaterTask.h"

HeaterTaskClass::HeaterTaskClass(int relayPin, int powerResolution, int percentagePower, int heaterTimeFrameInSeconds):
_lastState(LOW), _heaterTimeFrameInSeconds(heaterTimeFrameInSeconds), _percentagePower(percentagePower), _relayPin(relayPin),
_waitTime(_heaterTimeFrameInSeconds * 1000)
{
	_relayPin = relayPin;
	UpdateHeaterParameters(powerResolution, percentagePower, heaterTimeFrameInSeconds);

	pinMode(_relayPin, OUTPUT);
}

uint32_t HeaterTaskClass::timeOfNextCheck()
{
	setTriggered(true);
	return millisToMicros(_waitTime);
}

void HeaterTaskClass::exec()
{

	Serial.println("HeaterTask");
	
	if (_percentagePower > 0 && _lastState == LOW)
	{
		turnAndWait(_highTimeInMiliseconds, HIGH);
		return;
	}

	if (_percentagePower < 100 && _lastState == HIGH)
	{
		turnAndWait(_lowTimeInMiliseconds, LOW);
		return;
	}

	if (_percentagePower == 100)
	{
		turnAndWait(_heaterTimeFrameInSeconds * 1000, HIGH);
	}
	
	if (_percentagePower == 0)
	{
		turnAndWait(_heaterTimeFrameInSeconds * 1000, LOW);
	}
}

void HeaterTaskClass::UpdateHeaterParameters(int powerResolution, int percentagePower, int heaterTimeFrameInSeconds)
{
	_powerResolution = powerResolution;
	_percentagePower = percentagePower;
	_heaterTimeFrameInSeconds = heaterTimeFrameInSeconds;

	_highTimeInMiliseconds = _percentagePower * _heaterTimeFrameInSeconds * 1000 / _powerResolution;
	_lowTimeInMiliseconds = (_powerResolution - _percentagePower) * _heaterTimeFrameInSeconds * 1000 / _powerResolution;
}

void HeaterTaskClass::turnAndWait(int timeInMiliseconds, int state)
{
	digitalWrite(_relayPin, state);

	_lastState = state;
	_waitTime = timeInMiliseconds;
}
