#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <Arduino.h>

class SettingsClass
{
public:
    String wifiSsid = "";
    String wifiPassword = "";

    int relayPin = 0;
	int powerResolution = 0;
	int percentagePower = 0;
	int heaterTimeFrameInSeconds = 0;
    int tankSize = 0;
    int csvTimeFrameInSeconds = 0;

    int shelf10TemperatureLimit = 999;
    int headerTemperatureLimit = 999;
    int tankTemperatureLimit = 999;
    int waterTemperatureLimit = 999;

    bool saveCsv = false;
};

#endif // __SETTINGS_H__