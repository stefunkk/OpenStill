#include "ConfigurationService.h"

ConfigurationServiceClass::ConfigurationServiceClass(StillDataContextClass &context, SettingsClass &settings) : _context(context), _settings(settings)
{
}

void ConfigurationServiceClass::loadConfiguration()
{
    File file = SPIFFS.open(_fileName, "r");
    
    if (!file)
    {
        Serial.println(_fileName);
        Serial.println(F("Failed to read configuration file - FS error or file missing."));
        return;
    }
    StaticJsonDocument<1000> doc;

    DeserializationError error = deserializeJson(doc, file);
    Serial.println(error.c_str());
    if (error)
        Serial.println(F("Failed to read file, using default configuration"));

    // strlcpy(config.hostname,                 // <- destination
    //         doc["hostname"] | "example.com", // <- source
    //         sizeof(config.hostname));        // <- destination's capacity

    _settings.percentagePower = doc["percentagePower"];
    _settings.heaterTimeFrameInSeconds = doc["heaterTimeFrameInSeconds"];
    _settings.tankSize = doc["tankSize"];
    readArray("shelv10Address",doc,  _context.shelv10Address);
    readArray("headAddress",   doc,  _context.headAddress);
    readArray("tankAddress",   doc, _context.tankAddress);
    readArray("waterAddress",  doc,  _context.waterAddress);

    printFile(file);

    file.close();
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
    SPIFFS.remove(_fileName);

    Serial.println(F("Saving configuration"));

    File file = SPIFFS.open(_fileName, "w");
    if (!file)
    {
        Serial.println(F("Failed to create file"));
        return;
    }

    StaticJsonDocument<1000> doc;

    doc["percentagePower"] = _settings.percentagePower;
    doc["heaterTimeFrameInSeconds"] = _settings.heaterTimeFrameInSeconds;
    doc["tankSize"] = _settings.tankSize;

    JsonArray shelv10Data = doc.createNestedArray("shelv10Address");
    writeArray(shelv10Data, _context.shelv10Address);
    JsonArray headAddress = doc.createNestedArray("headAddress");
    writeArray(headAddress, _context.headAddress);
    JsonArray tankAddress = doc.createNestedArray("tankAddress");
    writeArray(tankAddress, _context.tankAddress);
    JsonArray waterAddress = doc.createNestedArray("waterAddress");
    writeArray(waterAddress, _context.waterAddress);

    if (serializeJson(doc, file) == 0)
    {
        Serial.println(F("Failed to write to file"));
    }
    
    printFile(file);
    
    file.close();
}

void ConfigurationServiceClass::printFile(File& file)
{
    file.seek(0, SeekSet);

	if (!file)
	{
		Serial.println(F("Failed to read file"));
		return;
	}

	while (file.available())
	{
		Serial.print((char)file.read());
	}
	Serial.println();
}

void ConfigurationServiceClass::writeArray(JsonArray &array, DeviceAddress &address)
{
    for (int i = 0; i < 8; i++)
    {
        array.add(address[i]);
    }
}
