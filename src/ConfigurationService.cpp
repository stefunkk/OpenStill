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

    StaticJsonDocument<1000> doc;

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

    readArray("shelf10Address", doc, _context.shelf10Address);
    readArray("headAddress", doc, _context.headAddress);
    readArray("tankAddress", doc, _context.tankAddress);
    readArray("waterAddress", doc, _context.waterAddress);
}

void ConfigurationServiceClass::readArray(String key, JsonDocument& doc, DeviceAddress& deviceAddress)
{
    for (int i = 0; i < 8; i++)
    {
        deviceAddress[i] = doc[key][i];
    }
}

void ConfigurationServiceClass::saveConfiguration()
{
    _fileService.removeFile(_fileName);

    StaticJsonDocument<1000> doc;

    doc["percentagePower"] = _settings.percentagePower;
    doc["heaterTimeFrameInSeconds"] = _settings.heaterTimeFrameInSeconds;
    doc["tankSize"] = _settings.tankSize;
    doc["csvTimeFrameInSeconds"] = _settings.csvTimeFrameInSeconds;

    doc["shelf10TemperatureLimit"] = _settings.shelf10TemperatureLimit;
    doc["headerTemperatureLimit"] = _settings.headerTemperatureLimit;
    doc["tankTemperatureLimit"] = _settings.tankTemperatureLimit;
    doc["waterTemperatureLimit"] = _settings.waterTemperatureLimit;

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
