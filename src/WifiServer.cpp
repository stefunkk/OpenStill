#include "WifiServer.h"

WifiServerClass::WifiServerClass(ESP8266WiFiClass &wifi, AsyncWebServer &server, SettingsClass &wifiSettings,
                                 SensorDataClass &sensorData, StillDataContextClass &context, ConfigurationServiceClass &configurationService) : _wifi(wifi), _server(server), _settings(wifiSettings), _sensorData(sensorData), _context(context), _configurationService(configurationService)
{
  configurePages();
  configureInputs();

  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Origin"), F("*"));
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Headers"), F("content-type"));
}

void WifiServerClass::connectToWifi()
{
  Serial.println(_settings.wifiSsid);
  Serial.println(_settings.wifiPassword);

  _wifi.mode(WIFI_STA);
  _wifi.begin(_settings.wifiSsid, _settings.wifiPassword);

  _server.begin();
}

void WifiServerClass::configurePages()
{
  _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", String(), false);
  });

  _server.on("/csv", HTTP_GET, [](AsyncWebServerRequest *request) {
    // auto response = request->beginResponse(200);
    // response->addHeader("Content-Disposition","attachment;filename=data.csv");
    request->send(SPIFFS, "/data.csv", "text/html", false);
  });

  _server.on("/data", HTTP_GET, [this](AsyncWebServerRequest *request) {
    char data[1000];
    sprintf(data, "{ ""\"shelf10\": %.02f, \"header\": %.02f, \"tank\": %.02f, \"water\": %.02f, \"heater\": %i }", 
    _sensorData.shelf10, _sensorData.header, _sensorData.tank, _sensorData.water, _settings.percentagePower);
    request->send(200, "application/json", data);
  });

  _server.on("/limitsData", HTTP_GET, [this](AsyncWebServerRequest *request) {
    char data[1000];
    sprintf(data, "{ "
      "\"shelf10TemperatureLimit\": %i, \"headerTemperatureLimit\": %i, \"tankTemperatureLimit\": %i, \"waterTemperatureLimit\": %i "
    "}", 
    _settings.shelf10TemperatureLimit, _settings.headerTemperatureLimit, _settings.tankTemperatureLimit, _settings.waterTemperatureLimit);
    request->send(200, "application/json", data);
  });

  _server.on("/sensors", HTTP_GET, [this](AsyncWebServerRequest *request) {
    char data[2500];

    sprintf(data,
            "[ { \"value\": %i, \"text\": \"%s\" } , "
            "  { \"value\": %i, \"text\": \"%s\" } , "
            "  { \"value\": %i, \"text\": \"%s\" } , "
            "  { \"value\": %i, \"text\": \"%s\" } ]",
            0, _context.foundDeviceString[0].c_str(),
            1, _context.foundDeviceString[1].c_str(),
            2, _context.foundDeviceString[2].c_str(),
            3, _context.foundDeviceString[3].c_str());

    request->send(200, "application/json", data);
  });
}

void WifiServerClass::UpdateDeviceAddress(String indexString, DeviceAddress &address)
{
  if (indexString.length() == 0)
  {
    Serial.println("Empty device index.");
    return;
  }

  auto index = indexString.toInt();

  if (index > 3)
  {
    Serial.println("Device index to high.");
    return;
  }

  for (int i = 0; i < 8; i++)
  {
    address[i] = _context.foundDevices[index][i];
  }

  Serial.println("Device index updated.");
}

void WifiServerClass::configureInputs()
{
  _server.on("/clearCsv", HTTP_GET, [this](AsyncWebServerRequest *request) {
    _context.clearCsv = true;
    request->send(200, "text/html");
  });

  _server.on("/setDevices", HTTP_GET, [this](AsyncWebServerRequest *request) {
    if (request->hasParam(_shelf10Device))
    {
      auto deviceIndex = request->getParam(_shelf10Device)->value();
      UpdateDeviceAddress(deviceIndex, _context.shelf10Address);
    }
    if (request->hasParam(_headerDevice))
    {
      auto deviceIndex = request->getParam(_headerDevice)->value();
      UpdateDeviceAddress(deviceIndex, _context.headAddress);
    }
    if (request->hasParam(_tankDevice))
    {
      auto deviceIndex = request->getParam(_tankDevice)->value();
      UpdateDeviceAddress(deviceIndex, _context.tankAddress);
    }
    if (request->hasParam(_waterDevice))
    {
      auto deviceIndex = request->getParam(_waterDevice)->value();
      UpdateDeviceAddress(deviceIndex, _context.waterAddress);
    }

    _configurationService.saveConfiguration();

    request->send(200, "text/html");
    return;
  });

  _server.on("/setLimits", HTTP_GET, [this](AsyncWebServerRequest *request) {
    if (request->hasParam(_shelf10TemperatureLimit))
    {
      int temperatureLimit = request->getParam(_shelf10TemperatureLimit)->value().toInt();
      _settings.shelf10TemperatureLimit = temperatureLimit;
    }
    if (request->hasParam(_headerTemperatureLimit))
    {
      int temperatureLimit = request->getParam(_headerTemperatureLimit)->value().toInt();
      _settings.headerTemperatureLimit = temperatureLimit;
    }
    if (request->hasParam(_tankTemperatureLimit))
    {
      int temperatureLimit = request->getParam(_tankTemperatureLimit)->value().toInt();
      _settings.tankTemperatureLimit = temperatureLimit;
    }
    if (request->hasParam(_waterTemperatureLimit))
    {
      int temperatureLimit = request->getParam(_waterTemperatureLimit)->value().toInt();
      _settings.waterTemperatureLimit = temperatureLimit;
    }
    
    _configurationService.saveConfiguration();

    request->send(200, "text/html");
    return;
  });

  _server.on("/set", HTTP_GET, [this](AsyncWebServerRequest *request) {
    if (request->hasParam(_heater))
    {
      int heatingPower = request->getParam(_heater)->value().toInt();
      if (heatingPower <= 100 && heatingPower >= 0)
      {
        _settings.percentagePower = heatingPower;
      }
    }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(_tankSize))
    {
      int tankSize = request->getParam(_heater)->value().toInt();
      if (tankSize <= 100 && tankSize >= 0)
      {
        _settings.tankSize = tankSize;
      }
    }
    else
    {
      request->send(404, "text/html");
      return;
    }

    _configurationService.saveConfiguration();

    request->send(200, "text/html");
    return;
  });
}