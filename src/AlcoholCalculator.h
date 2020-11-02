// AlcoholCalculator.h

#ifndef _ALCOHOLCALCULATOR_h
#define _ALCOHOLCALCULATOR_h

#include <Arduino.h>

class AlcoholCalculatorClass
{
 public:
	static double calculateWashBoilingTemperatureByAbv(float alcoholPercentage);
	static double calculateAbvByHeadVapourTemperature(float vapourTemperature);
	static double calculateAbvByWashBoilingTemperature(float temperature);
	static double calculateAlcoholVolumeByWashBoilingTemperature(float temperature, float washVolume);
	static double const ERROR;
};

#endif

