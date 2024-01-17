#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <Arduino.h>

class SettingsClass
{
public:
    String wifiSsid = "";
    String wifiPassword = "";

    int relayPin1 = 0;
    int relayPin2 = 0;
    int relayPin3 = 0;

    int heater2State = LOW;
    bool heater2StateChanged = false;

    bool heater3State = LOW;
    bool heater3StateChanged = false;

	int powerResolution = 0;
	int percentagePower = 0;
	int heaterTimeFrameInSeconds = 0;
    int tankSize = 0;
    int csvTimeFrameInSeconds = 0;
    
    char pushNotificationCode[100]= "";

    int shelf10TemperatureLimit = 999;
    int headerTemperatureLimit = 999;
    int tankTemperatureLimit = 999;
    int waterTemperatureLimit = 999;

    int shelf10TemperatureNotification = 999;
    int headerTemperatureNotification = 999;
    int tankTemperatureNotification = 999;
    int waterTemperatureNotification = 999;

    bool saveCsv = false;

    double scaleOffset = 0;
    int scaleWeightNotification = 0;

    int sckPin = 0;
    int doutPin = 0;
};

#endif // __SETTINGS_H__