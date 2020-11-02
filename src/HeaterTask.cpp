#include "HeaterTask.h"

HeaterTaskClass::HeaterTaskClass(SettingsClass& settingsClass):
	_settingsClass(settingsClass), _lastState(LOW)
{
	pinMode(_settingsClass.relayPin, OUTPUT);
}

uint32_t HeaterTaskClass::timeOfNextCheck()
{
	setTriggered(true);
	return millisToMicros(_waitTime);
}

void HeaterTaskClass::exec()
{

	Serial.println("HeaterTask");
	
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
	digitalWrite(_settingsClass.relayPin, state);

	_lastState = state;
	_waitTime = timeInMiliseconds;
}

int HeaterTaskClass::getHighTime() 
{
	return _settingsClass.percentagePower * _settingsClass.heaterTimeFrameInSeconds * 1000 / _settingsClass.powerResolution;
}

int HeaterTaskClass::getLowTime() 
{
	return (_settingsClass.powerResolution - _settingsClass.percentagePower) * _settingsClass.heaterTimeFrameInSeconds * 1000 / _settingsClass.powerResolution;
}
