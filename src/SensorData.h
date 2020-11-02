// SensorData.h

#ifndef _SENSORDATA_h
#define _SENSORDATA_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <list>

class SensorDataClass
{
 public:
	 double header = 0;
	 double shelf10 = 0;
	 double tank = 0;
	 double water = 0;
};

extern SensorDataClass SensorData;

#endif

