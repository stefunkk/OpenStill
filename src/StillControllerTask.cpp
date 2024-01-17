#include "StillControllerTask.h"

StillControllerTaskClass::StillControllerTaskClass(SensorDataClass &sensorData, SettingsClass &settings, StillDataContextClass &context) : _sensorData(sensorData), _settings(settings), _context(context)
{
}

void StillControllerTaskClass::exec()
{
	checkTemperatureLimit();
	checkTempOfTheDay();
	checkTempNotifications();
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

void StillControllerTaskClass::checkTempNotifications() 
{
	if (_sensorData.shelf10 > _settings.shelf10TemperatureNotification && !_context.shelf10TemperatureNotificationSent)
	{
		NotificationHelperClass::addNotification(_context, "OpenStill", "Temperatura 10 półki (" + String(_settings.shelf10TemperatureNotification)  + "*C) osiągnięta.");
		_context.shelf10TemperatureNotificationSent = true;
	}
	if (_sensorData.header > _settings.headerTemperatureNotification && !_context.headerTemperatureNotificationSent)
	{
		NotificationHelperClass::addNotification(_context, "OpenStill", "Temperatura głowicy (" + String(_settings.headerTemperatureNotification)  + "*C) osiągnięta.");
		_context.headerTemperatureNotificationSent = true;
	}
	if (_sensorData.tank > _settings.tankTemperatureNotification && !_context.tankTemperatureNotificationSent)
	{
		NotificationHelperClass::addNotification(_context, "OpenStill", "Temperatura zbiornika (" + String(_settings.tankTemperatureNotification)  + "*C) osiągnięta.");
		_context.tankTemperatureNotificationSent = true;
	}
	if (_sensorData.water > _settings.waterTemperatureNotification && !_context.waterTemperatureNotificationSent)
	{
		NotificationHelperClass::addNotification(_context, "OpenStill", "Temperatura wody (" + String(_settings.waterTemperatureNotification)  + "*C) osiągnięta.");
		_context.waterTemperatureNotificationSent = true;
	}
	if (_context.weight > _settings.scaleWeightNotification && !_context.scaleWeightNotificationSent)
	{
		NotificationHelperClass::addNotification(_context, "OpenStill", "Waga (" + String(_settings.scaleWeightNotification)  + "g) została osiągnięta.");
		_context.scaleWeightNotificationSent = true;
	}
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
