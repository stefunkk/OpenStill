#include "LcdService.h"
#include <LiquidCrystalIO.h>
#include <IoAbstractionWire.h>
#include <Wire.h>

LiquidCrystalI2C_RS_EN(lcd, 0x3f, false)

LcdServiceClass::LcdServiceClass(AlcoholCalculatorClass& alcoholCalculator,
                                 char(&names)[4][5], float tankSize) :
	_lcd(*lcd), _alcoholCalculator(alcoholCalculator), _names(names), _temperatures{0, 0, 0, 0}
{
	_tankSize = tankSize;

	
	_lcd.configureBacklightPin(3);
	_lcd.backlight();
	Wire.begin();
	_lcd.begin(20, 4);
	
	_lcd.print("OpenStill");
	createCustomCharacters();
	
	_lcd.write(0);
	_lcd.write(1);
}

void LcdServiceClass::printTemperatures(SensorDataClass &data)
{
	double temperatures[4] = { data.shelf10, data.header, data.tank, data.water };
	
	for (int i = 0; i<4; i++)
	{
		double temp = temperatures[i];

		if (temp == _temperatures[i])
		{
			Serial.print(_names[i]);
			Serial.print("(pomijam): ");
			Serial.println(temp);
			continue;
		}
		
		_lcd.setCursor(0, i);

		_lcd.printf("%4.4s:", _names[i]);


		if (temp < 0)
		{
			temp = 0;
		}

		Serial.print(_names[i]);
		Serial.print(": ");
		Serial.println(temp);
		_lcd.printf("%6.2f", temp);

		if (temp != _temperatures[i])
		{
			printTrending(temp, i);
		}

		_temperatures[i] = temp;

	}

	printExtraData(temperatures);
}

void LcdServiceClass::printTrending(float temperature, int counter) const
{
	int displayChar;

	if (temperature > _temperatures[counter])
	{
		displayChar = 0;
	}
	else if (temperature < _temperatures[counter])
	{
		displayChar = 1;
	}

	_lcd.write(displayChar);
}

void LcdServiceClass::printExtraData(const double temperatures[4]) const
{
	printHeadAbv(temperatures[1]);
	printTankAlcoholLeft(temperatures[2]);
}

void LcdServiceClass::printHeadAbv(float temperature) const
{
	_lcd.setCursor(14, _headIndex);
	double headAbv = _alcoholCalculator.calculateAbvByHeadVapourTemperature(temperature);

	if (headAbv != AlcoholCalculatorClass::ERROR)
	{
		_lcd.write(3);
		_lcd.printf("%4.1f", headAbv);
		_lcd.print("%");
	}
	else
	{
		_lcd.print("      ");
	}
}

void LcdServiceClass::printTankAlcoholLeft(float temperature) const
{
	_lcd.setCursor(14, _tankIndex);
	const double tankAlcoholLeft = _alcoholCalculator.calculateAlcoholVolumeByWashBoilingTemperature(
		temperature, _tankSize);

	if (tankAlcoholLeft != AlcoholCalculatorClass::ERROR)
	{
		_lcd.write(2);
		_lcd.printf("%-4.1f", tankAlcoholLeft);
		_lcd.print("L");
	}
	else
	{
		_lcd.print("      ");
	}
}

void LcdServiceClass::createCustomCharacters() const
{
	byte arrowUp[8] = {
	0b00100,
	0b01110,
	0b11111,
	0b00100,
	0b00100,
	0b00100,
	0b00100,
	0b00100
	};

	byte arrowDown[8] = {
	0b00100,
	0b00100,
	0b00100,
	0b00100,
	0b00100,
	0b11111,
	0b01110,
	0b00100
	};
	
	byte tank[8] = {
	0b01110,
	0b10001,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b01110
	};
	
	byte head[8] = {
	0b11111,
	0b11111,
	0b11000,
	0b11000,
	0b11000,
	0b11000,
	0b11000,
	0b11000
	};

	_lcd.createCharPgm(0, arrowUp);
	_lcd.createCharPgm(1, arrowDown);
	_lcd.createCharPgm(2, tank);
	_lcd.createCharPgm(3, head);
}
