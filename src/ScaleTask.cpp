#include "ScaleTask.h"

ScaleTaskClass::ScaleTaskClass(StillDataContextClass &context, HX711 &scale, SettingsClass &settings) : _context(context), _scale(scale), _settings(settings)
{
	_scale.begin(_settings.doutPin, _settings.sckPin);

	if(_settings.scaleOffset > 0)
	{
		_scale.set_scale(_settings.scaleOffset);
	}
	else 
	{
		_scale.set_scale();
	}
	
	_scale.tare();
}

uint32_t ScaleTaskClass::timeOfNextCheck()
{
	setTriggered(true);
	return millisToMicros(200);
}

void ScaleTaskClass::CalculateFlowRate(double weight, unsigned long readingTime)
{
	if (lastWeightReadingTime == 0)
	{
		lastWeightReadingTime = readingTime;
		lastWeightReading = weight;
	}

	auto timePassed = readingTime - lastWeightReadingTime;
	if (timePassed > flowRateDelay)
	{
		auto weightChange = weight - lastWeightReading;

		_context.flowRate = weightChange * 60000 / timePassed;
	}
	else 
	{
		return;
	}

	lastWeightReadingTime = readingTime;
	lastWeightReading = weight;
}

void ScaleTaskClass::exec()
{
	if (_context.scaleOffsetChanged)
	{
		_scale.set_scale(_settings.scaleOffset);
		_scale.tare();

		_context.scaleOffsetChanged = false;
	}

	if (_context.shouldTare)
	{
		Serial.println("Tare.");

		_scale.tare();
		_context.weight = _scale.get_units(5);
		_context.shouldTare = false;
	}

	if (_scale.wait_ready_timeout(200))
	{
		_context.weight = _scale.get_units(5);
		CalculateFlowRate(_context.weight, millis());
	}
}
