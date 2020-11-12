#include "HeaterTask.h"

HeaterTaskClass::HeaterTaskClass(SettingsClass &settingsClass) : _settingsClass(settingsClass), _lastState(LOW)
{
	pinMode(_settingsClass.relayPin1, OUTPUT);
	pinMode(_settingsClass.relayPin2, OUTPUT);
	pinMode(_settingsClass.relayPin3, OUTPUT);

	digitalWrite(_settingsClass.relayPin1, LOW);
	digitalWrite(_settingsClass.relayPin2, LOW);
	digitalWrite(_settingsClass.relayPin3, LOW);
}

uint32_t HeaterTaskClass::timeOfNextCheck()
{
	setTriggered(true);
	return millisToMicros(_waitTime);
}

void HeaterTaskClass::exec()
{
	if (_settingsClass.heater2StateChanged)
	{
		digitalWrite(_settingsClass.relayPin2, _settingsClass.heater2State);
		_settingsClass.heater2StateChanged = false;
	}

	if (_settingsClass.heater3StateChanged)
	{
		digitalWrite(_settingsClass.relayPin3, _settingsClass.heater3State);
		_settingsClass.heater3StateChanged = false;
	}

	if (_settingsClass.percentagePower > 0 && _lastState == LOW)
	{
		turnAndWait(getHighTime(), HIGH);
		return;
	}

	if (_settingsClass.percentagePower < 100 && _lastState == HIGH)
	{
		turnAndWait(getLowTime(), LOW);
		return;
	}

	if (_settingsClass.percentagePower == 100)
	{
		turnAndWait(_settingsClass.heaterTimeFrameInSeconds * 1000, HIGH);
	}

	if (_settingsClass.percentagePower == 0)
	{
		turnAndWait(_settingsClass.heaterTimeFrameInSeconds * 1000, LOW);
	}
}

void HeaterTaskClass::turnAndWait(int timeInMiliseconds, int state)
{
	digitalWrite(_settingsClass.relayPin1, state);

	_lastState = state;
	_waitTime = timeInMiliseconds;
}

int HeaterTaskClass::getLowTime()
{
	return _settingsClass.percentagePower * _settingsClass.heaterTimeFrameInSeconds * 1000 / _settingsClass.powerResolution;
}

int HeaterTaskClass::getHighTime()
{
	return (_settingsClass.powerResolution - _settingsClass.percentagePower) * _settingsClass.heaterTimeFrameInSeconds * 1000 / _settingsClass.powerResolution;
}
