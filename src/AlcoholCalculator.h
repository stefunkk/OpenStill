// AlcoholCalculator.h

#ifndef _ALCOHOLCALCULATOR_h
#define _ALCOHOLCALCULATOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class AlcoholCalculatorClass
{
 public:
	static double calculateWashBoilingTemperatureByAbv(float alcoholPercentage);
	static double calculateAbvByHeadVapourTemperature(float vapourTemperature);
	static double calculateAbvByWashBoilingTemperature(float temperature);
	static double calculateAlcoholVolumeByWashBoilingTemperature(float temperature, float washVolume);
	static double const ERROR;
};

extern AlcoholCalculatorClass AlcoholCalculator;

#endif

