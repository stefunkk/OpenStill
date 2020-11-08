#include "LcdTask.h"

LcdTaskClass::LcdTaskClass(StillDataContextClass& context, SensorDataClass& sensorData, LcdServiceClass& lcdService):
	_context(context), _sensorData(sensorData), _lcdService(lcdService)
{
}

uint32_t LcdTaskClass::timeOfNextCheck()
{
	setTriggered(true);
	return millisToMicros(1000);
}

void LcdTaskClass::exec()
{
	if (!_context.isIpShown && _context.ipAddress != ""){
		_lcdService.printIpAddress(_context.ipAddress);
		_context.isIpShown = true;
		_shouldReset = true;
		return;
	}

	if (_shouldReset){
		_lcdService.reset();
		_shouldReset = false;
	}

	_lcdService.printTemperatures(_sensorData);
}

