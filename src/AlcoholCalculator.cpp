// Thanks to https://homedistiller.org/

#include "AlcoholCalculator.h"

double AlcoholCalculatorClass::calculateWashBoilingTemperatureByAbv(float alcoholPercentage)
{
	const auto percentage = alcoholPercentage / 100;

	const double A = 60.526;
	const double B = 163.16;
	const double C = 163.96;
	const double D = 83.438;

	const auto stage =
		(A * pow(percentage, 4) - B * pow(percentage, 3) + C * pow(percentage, 2) - D * percentage +
			100) * 10;
	const auto result = round(stage) / 10;

	return result;
}

double AlcoholCalculatorClass::calculateAbvByHeadVapourTemperature(float vapourTemperature)
{
	if (vapourTemperature < 78 || vapourTemperature > 100)
	{
		return ERROR;
	}

	const double A = 8.4868916245560797E+01;
	const double B = -1.7679487852159399E-02;
	const double C = 1.6500287186786501E-06;
	const double D = -5.8236674277005885E-11;
	const double E = -1.4829038599925209E+05;
	const double F = 2.9150067221926470E+03;
	const double J = 2.5124863462680742E+00;
	const double K = 5.9226708251803248E+03;
	
	auto result = (A + B * pow(vapourTemperature,2) + C * pow(vapourTemperature, 4) + D * 
		pow(vapourTemperature, 6) + E / (pow(vapourTemperature,2) - J) + F / pow((pow(vapourTemperature,2) - K), 2))*100;
	
	return result;
}

double AlcoholCalculatorClass::calculateAbvByWashBoilingTemperature(float temperature)
{
	if (temperature < 78 || temperature > 100)
	{
		return ERROR;
	}
	
	const double A = 234.1002;
	const double B = 9.12097;
	const double C = 0.133522;
	const double D = 0.00086943;
	const double E = 0.00000212211;
	
	const auto result = (A - B * temperature + C * pow(temperature, 2) - D *
		pow(temperature, 3) + E * pow(temperature, 4)) * 100;

	return result;
}

double AlcoholCalculatorClass::calculateAlcoholVolumeByWashBoilingTemperature(float temperature, float washVolume)
{
	const auto percentage = calculateAbvByWashBoilingTemperature(temperature);

	if (percentage == ERROR)
	{
		return ERROR;
	}

	return percentage / 100 * washVolume;
}

const double AlcoholCalculatorClass::ERROR = -127;