#include "LcdTask.h"

LcdTaskClass::LcdTaskClass(SensorDataClass& sensorData, LcdServiceClass& lcdService):
	_sensorData(sensorData), _lcdService(lcdService)
{
}

uint32_t LcdTaskClass::timeOfNextCheck()
{
	setTriggered(true);
	return millisToMicros(1000);
}

void LcdTaskClass::exec()
{
	_lcdService.printTemperatures(_sensorData);
}

