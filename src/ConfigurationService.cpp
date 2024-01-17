#include "ConfigurationService.h"

ConfigurationServiceClass::ConfigurationServiceClass(FileServiceClass &fileService, StillDataContextClass &context, SettingsClass &settings) : _context(context), _settings(settings), _fileService(fileService)
{
}

void ConfigurationServiceClass::loadConfiguration()
{
    auto fileContent = _fileService.openFile(_fileName);

    if (fileContent.length() == 0)
    {
        return;
    }

    StaticJsonDocument<1500> doc;

    DeserializationError error = deserializeJson(doc, fileContent);

    if (error)
    {
        Serial.println(error.c_str());
        Serial.println(F("Failed to read file, using default configuration"));
    }

    _settings.percentagePower = doc["percentagePower"];
    _settings.heaterTimeFrameInSeconds = doc["heaterTimeFrameInSeconds"];
    _settings.tankSize = doc["tankSize"];
    _settings.csvTimeFrameInSeconds = doc["csvTimeFrameInSeconds"];

    _settings.shelf10TemperatureLimit = doc["shelf10TemperatureLimit"];
    _settings.headerTemperatureLimit = doc["headerTemperatureLimit"];
    _settings.tankTemperatureLimit = doc["tankTemperatureLimit"];
    _settings.waterTemperatureLimit = doc["waterTemperatureLimit"];

    _settings.shelf10TemperatureNotification = doc["shelf10TemperatureNotification"];
    _settings.headerTemperatureNotification = doc["headerTemperatureNotification"];
    _settings.tankTemperatureNotification = doc["tankTemperatureNotification"];
    _settings.waterTemperatureNotification = doc["waterTemperatureNotification"];

    _settings.scaleWeightNotification = doc["scaleWeightNotification"];

    _settings.wifiSsid = doc["wifiSsid"].as<String>();
    _settings.wifiPassword = doc["wifiPassword"].as<String>();

    _settings.scaleOffset = doc["scaleOffset"];

    strlcpy(_settings.pushNotificationCode, doc["pushNotificationCode"], sizeof(_settings.pushNotificationCode));

    readArray("shelf10Address", doc, _context.shelf10Address);
    readArray("headAddress", doc, _context.headAddress);
    readArray("tankAddress", doc, _context.tankAddress);
    readArray("waterAddress", doc, _context.waterAddress);
}

void ConfigurationServiceClass::readArray(String key, JsonDocument &doc, DeviceAddress &deviceAddress)
{
    for (int i = 0; i < 8; i++)
    {
        deviceAddress[i] = doc[key][i];
    }
}

void ConfigurationServiceClass::saveConfiguration()
{
    _fileService.removeFile(_fileName);

    StaticJsonDocument<1500> doc;

    doc["percentagePower"] = _settings.percentagePower;
    doc["heaterTimeFrameInSeconds"] = _settings.heaterTimeFrameInSeconds;
    doc["tankSize"] = _settings.tankSize;
    doc["csvTimeFrameInSeconds"] = _settings.csvTimeFrameInSeconds;
    doc["pushNotificationCode"] = _settings.pushNotificationCode;

    doc["shelf10TemperatureLimit"] = _settings.shelf10TemperatureLimit;
    doc["headerTemperatureLimit"] = _settings.headerTemperatureLimit;
    doc["tankTemperatureLimit"] = _settings.tankTemperatureLimit;
    doc["waterTemperatureLimit"] = _settings.waterTemperatureLimit;

    doc["shelf10TemperatureNotification"] = _settings.shelf10TemperatureNotification;
    doc["headerTemperatureNotification"] = _settings.headerTemperatureNotification;
    doc["tankTemperatureNotification"] = _settings.tankTemperatureNotification;
    doc["waterTemperatureNotification"] = _settings.waterTemperatureNotification;

    doc["scaleWeightNotification"] = _settings.scaleWeightNotification;

    doc["wifiSsid"] = _settings.wifiSsid;
    doc["wifiPassword"] = _settings.wifiPassword;

    doc["scaleOffset"] = _settings.scaleOffset;

    JsonArray shelf10Data = doc.createNestedArray("shelf10Address");
    writeArray(shelf10Data, _context.shelf10Address);
    JsonArray headAddress = doc.createNestedArray("headAddress");
    writeArray(headAddress, _context.headAddress);
    JsonArray tankAddress = doc.createNestedArray("tankAddress");
    writeArray(tankAddress, _context.tankAddress);
    JsonArray waterAddress = doc.createNestedArray("waterAddress");
    writeArray(waterAddress, _context.waterAddress);

    String content;
    if (serializeJson(doc, content) == 0)
    {
        Serial.println(F("Failed to write to file"));
    }
    
    _fileService.saveFile(_fileName, content);
}

void ConfigurationServiceClass::writeArray(JsonArray &array, DeviceAddress &address)
{
    for (int i = 0; i < 8; i++)
    {
        array.add(address[i]);
    }
}
