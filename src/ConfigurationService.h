#ifndef __CONFIGURATIONSERVICE_H__
#define __CONFIGURATIONSERVICE_H__
#include "StillDataContext.h"
#include "Settings.h"
#include <ArduinoJson.h>
#include <Arduino.h>
#include <SPI.h>
#include "FileService.h"

class ConfigurationServiceClass
{
public:
    ConfigurationServiceClass(FileServiceClass &fileService, StillDataContextClass &context, SettingsClass &settings);
    void loadConfiguration();
    void saveConfiguration();

private:
    void readArray(String key, JsonDocument &doc, DeviceAddress &deviceAddress);
    void writeArray(JsonArray &array, DeviceAddress &address);
    StillDataContextClass &_context;
    SettingsClass &_settings;
    String _fileName = "/config.json";
    FileServiceClass &_fileService;
};

#endif // __CONFIGURATIONSERVICE_H__