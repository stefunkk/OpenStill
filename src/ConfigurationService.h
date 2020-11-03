#ifndef __CONFIGURATIONSERVICE_H__
#define __CONFIGURATIONSERVICE_H__
#include "StillDataContext.h"
#include "Settings.h"
#include <ArduinoJson.h>
#include <Arduino.h>
#include <SPI.h>
#include <FS.h>

class ConfigurationServiceClass
{
public:
    ConfigurationServiceClass(StillDataContextClass &context, SettingsClass &settings);
    void loadConfiguration();
    void saveConfiguration();
private:
    void readArray(String key, JsonDocument& doc, DeviceAddress &deviceAddress);
    void printFile(File &file);
    void writeArray(JsonArray &array, DeviceAddress &address);
    StillDataContextClass &_context;
    SettingsClass &_settings;
    String _fileName = "/config.json";
};

#endif // __CONFIGURATIONSERVICE_H__