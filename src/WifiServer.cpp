#include "WifiServer.h"

WifiServerClass::WifiServerClass(ESP8266WiFiClass &wifi, AsyncWebServer &server, SettingsClass &wifiSettings, SensorDataClass &sensorData) : _wifi(wifi), _server(server), _settings(wifiSettings), _sensorData(sensorData)
{
  connectToWifi();
  configureLittleFS();
  configurePages();
  configureInputs();
  
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Origin"), F("*"));
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Headers"), F("content-type"));
}

void WifiServerClass::connectToWifi()
{
  _wifi.mode(WIFI_STA);
  _wifi.begin(_settings.wifiSsid, _settings.wifiPassword);

  _server.begin();
}

void WifiServerClass::configureLittleFS()
{
  if (!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
}

void WifiServerClass::configurePages()
{
  _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", String(), false);
  });

  _server.on("/data", HTTP_GET, [this](AsyncWebServerRequest *request) {
    char data[1000];
    sprintf(data, "{ \"shelf10\": %.02f, \"header\": %.02f, \"tank\": %.02f, \"water\": %.02f, \"heater\": %i }", _sensorData.shelf10, _sensorData.header, _sensorData.tank, _sensorData.water, _settings.percentagePower);
    request->send(200, "application/json", data);
  });
}

void WifiServerClass::configureInputs()
{
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

    request->send(200, "text/html");
    return;
  });
}