#ifndef __WIFISETTINGS_H__
#define __WIFISETTINGS_H__
#include <Arduino.h>

class SettingsClass
{
public:
    String wifiSsid = "",
     wifiPassword = "";

    int relayPin = 0,
	powerResolution = 0,
	percentagePower = 0,
	heaterTimeFrameInSeconds = 0,
    tankSize = 0;
};

#endif // __WIFISETTINGS_H__