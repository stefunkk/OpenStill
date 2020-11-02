// LcdService.h
#ifndef _LCDSERVICE_h
#define _LCDSERVICE_h

#include <LiquidCrystalIO.h>
#include "AlcoholCalculator.h"
#include "SensorData.h"


class LcdServiceClass
{
public:
	void createCustomCharacters() const;
	LcdServiceClass(AlcoholCalculatorClass& alcoholCalculator, const char (&names)[4][5], float tankSize);
	void printTrending(float temperature, int counter) const;
	void printTemperatures(SensorDataClass &data);
private:
	void printHeadAbv(float temperature) const;
	void printTankAlcoholLeft(float temperature) const;
	auto printExtraData(const double temperatures[4]) const -> void;
	LiquidCrystal& _lcd;
	int _tankIndex = 2;
	int _headIndex = 1;
	AlcoholCalculatorClass& _alcoholCalculator;
	const char (&_names)[4][5];
	float _tankSize;
	float _temperatures[4];
};

#endif
