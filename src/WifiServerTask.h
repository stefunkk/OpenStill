// WifiServerTask.h

#ifndef _WIFISERVERTASK_h
#define _WIFISERVERTASK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <ESP8266WiFi.h>
#include <TaskManagerIO.h>

class WifiServerTaskClass: public BaseEvent
{
 public:
	 WifiServerTaskClass(ESP8266WiFiClass& wifi, WiFiServer& server);
	 void exec() override;
	 uint32_t timeOfNextCheck() override;
private:
	ESP8266WiFiClass& _wifi;
	WiFiServer& _server;
};

extern WifiServerTaskClass WifiServerTask;

#endif

