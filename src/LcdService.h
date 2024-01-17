// LcdService.h
#ifndef _LCDSERVICE_h
#define _LCDSERVICE_h

#include <LiquidCrystalIO.h>
#include "AlcoholCalculator.h"
#include "SensorData.h"
#include "StillDataContext.h"

class LcdServiceClass
{
public:
	void createCustomCharacters() const;
	LcdServiceClass(AlcoholCalculatorClass &alcoholCalculator, StillDataContextClass &context, float tankSize);
	void printTrending(float temperature, int counter) const;
	void printTemperatures(SensorDataClass &data);
	void printIpAddress(String ip);
	void reset();

private:
	void printHeadAbv(float temperature) const;
	void printTankAlcoholLeft(float temperature) const;
	auto printExtraData(const double temperatures[4]) const -> void;
	auto printWeight(const double weight) const -> void;
	auto printFlowRateWeight(double flowRate) const -> void;
	LiquidCrystal &_lcd;
	int _tankIndex = 1;
	int _headIndex = 0;
	int _weightIndex = 2;
	int _flowRateIndex = 3;
	AlcoholCalculatorClass &_alcoholCalculator;
	float _tankSize;
	float _temperatures[4];
	StillDataContextClass &_context;
};

#endif
