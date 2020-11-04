#include "StillControllerTask.h"

StillControllerTaskClass::StillControllerTaskClass(SensorDataClass& sensorData, SettingsClass& settings, StillDataContextClass& context):
    _sensorData(sensorData), _settings(settings), _context(context)
{
    
}

void StillControllerTaskClass::exec() 
{
    if (isTemperatureLimitReached())
	{
		_settings.percentagePower = 0;
	}
}

bool StillControllerTaskClass::isTemperatureLimitReached(){
	return 
		_settings.shelf10TemperatureLimit < _sensorData.shelf10 ||
		_settings.headerTemperatureLimit < _sensorData.header ||
		_settings.tankTemperatureLimit < _sensorData.tank ||
		_settings.waterTemperatureLimit < _sensorData.water;

}

uint32_t StillControllerTaskClass::timeOfNextCheck() 
{
	setTriggered(true);
	return millisToMicros(5000);
}
