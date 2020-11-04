#include "StillControllerTask.h"

StillControllerTaskClass::StillControllerTaskClass(SensorDataClass &sensorData, SettingsClass &settings, StillDataContextClass &context) : _sensorData(sensorData), _settings(settings), _context(context)
{
}

void StillControllerTaskClass::exec()
{
	checkTemperatureLimit();
	checkTempOfTheDay();
}

void StillControllerTaskClass::checkTemperatureLimit()
{
	if (isTemperatureLimitReached())
	{
		if (_settings.percentagePower > 0)
		{
			NotificationHelperClass::addNotification(_context, "OpenStill", "Limit temperatury przekroczony - wyłączam grzałkę.");
		}
		_settings.percentagePower = 0;
	}
}

bool StillControllerTaskClass::isTemperatureLimitReached()
{
	return _settings.shelf10TemperatureLimit < _sensorData.shelf10 ||
		   _settings.headerTemperatureLimit < _sensorData.header ||
		   _settings.tankTemperatureLimit < _sensorData.tank ||
		   _settings.waterTemperatureLimit < _sensorData.water;
}

uint32_t StillControllerTaskClass::timeOfNextCheck()
{
	setTriggered(true);
	return millisToMicros(5000);
}

void StillControllerTaskClass::checkTempOfTheDay()
{
	if (_context.tempOfTheDay == 0)
	{
		return;
	}

	auto tempDeviation = _sensorData.shelf10 - _context.tempOfTheDay;
	auto timeFromLastNotification = millis() - _context.tempofTheDayNotificationTime;

	if (tempDeviation > _context.tempOfTheDayDeviation && timeFromLastNotification > (_context.tempOfTheDayNotificationDelayInSeconds * 1000))
	{
		NotificationHelperClass::addNotification(_context, "OpenStill", "Temperatura dnia przekroczona o " + String(tempDeviation));
		_context.tempofTheDayNotificationTime = millis();
	}
}
