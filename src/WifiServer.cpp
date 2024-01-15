#include "WifiServer.h"

WifiServerClass::WifiServerClass(AsyncWebServer &server, SettingsClass &wifiSettings,
                                 SensorDataClass &sensorData, StillDataContextClass &context, ConfigurationServiceClass &configurationService) 
                                 : _server(server), _settings(wifiSettings), _sensorData(sensorData), _context(context), _configurationService(configurationService)
{

  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Origin"), F("*"));
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Headers"), F("content-type"));
}

void WifiServerClass::connectToWifi()
{
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(_settings.wifiSsid.c_str(), _settings.wifiPassword.c_str());

  _server.begin();

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  _context.ipAddress = WiFi.localIP().toString();

  Serial.print("IP address: ");
  Serial.print(_context.ipAddress);

  configurePages();
  configureInputs();
}

void WifiServerClass::setupAccessPoint()
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  IPAddress localIp(192, 168, 4, 1);
  IPAddress gateway(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);

  WiFi.softAPConfig(localIp, gateway, subnet);
  WiFi.softAP("OpenStill");

  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  _server.begin();

  configureAPPages();
  configureInputs();
}

void WifiServerClass::configureAPPages()
{
  _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    const char index_html[] PROGMEM = "<html> <head> <meta charset=\"UTF-8\"> </head> <body> <form v-if=\"false\" action=\"/set\" method=\"get\"> <label for=\"wifiSsid\">Wifi SSID</label> <input type=\"text\" name=\"wifiSsid\"><br /><br /> <label for=\"wifiPassword\">Wifi Pass</label> <input type=\"text\" name=\"wifiPassword\"><br /><br /> <input type=\"submit\" value=\"Ok\"> </form> </body> </html>";
    request->send_P(200, "text/html", index_html);
  });
}

void WifiServerClass::configurePages()
{
  _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", String(), false);
  });

  _server.on("/csv", HTTP_GET, [](AsyncWebServerRequest *request) {
    // auto response = request->beginResponse(200);
    // response->addHeader("Content-Disposition","attachment;filename=data.csv");
    request->send(SPIFFS, "/data.csv", "text/csv", false);
  });

  _server.on("/data", HTTP_GET, [this](AsyncWebServerRequest *request) {
    char data[1000];
    sprintf(data, "{ "
                  "\"shelf10\": %.02f, \"header\": %.02f, \"tank\": %.02f, \"water\": %.02f, \"heater\": %i, "
                  "\"tankAbw\": %.02f, \"headerAbv\": %.02f1, "
                  "\"heater2\": %i, \"heater3\": %i"
                  "}",
            _sensorData.shelf10, _sensorData.header, _sensorData.tank, _sensorData.water, _settings.percentagePower,
            AlcoholCalculatorClass::calculateAlcoholVolumeByWashBoilingTemperature(_sensorData.tank, _settings.tankSize), AlcoholCalculatorClass::calculateAbvByHeadVapourTemperature(_sensorData.header),
            _settings.heater2State, _settings.heater3State);
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

  _server.on("/notificationsData", HTTP_GET, [this](AsyncWebServerRequest *request) {
    char data[1000];
    sprintf(data, "{ "
                  "\"shelf10TemperatureNotification\": %i, \"headerTemperatureNotification\": %i, \"tankTemperatureNotification\": %i, \"waterTemperatureNotification\": %i "
                  "}",
            _settings.shelf10TemperatureNotification, _settings.headerTemperatureNotification, _settings.tankTemperatureNotification, _settings.waterTemperatureNotification);
    request->send(200, "application/json", data);
  });

  _server.on("/otherConfiguration", HTTP_GET, [this](AsyncWebServerRequest *request) {
    char data[1000];
    sprintf(data, "{ "
                  "\"tankSize\": %i, \"csvTimeFrameInSeconds\": %i, \"pushNotificationCode\": \"%s\","
                  "\"tempOfTheDayDeviation\": %.02f, \"tempOfTheDayNotificationDelayInSeconds\": %i, \"tempOfTheDay\": %.02f,"
                  "\"wifiSsid\": \"%s\", \"wifiPassword\": \"%s\""
                  "}",
            _settings.tankSize, _settings.csvTimeFrameInSeconds, _settings.pushNotificationCode,
            _context.tempOfTheDayDeviation, _context.tempOfTheDayNotificationDelayInSeconds, _context.tempOfTheDay,
            _settings.wifiSsid.c_str(), _settings.wifiPassword.c_str());
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

  _server.on("/notificationTest", HTTP_GET, [this](AsyncWebServerRequest *request) {
    NotificationHelperClass::addNotification(_context, "OpenStill", "Testowe powiadomienie");
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

  _server.on("/setNotifications", HTTP_GET, [this](AsyncWebServerRequest *request) {
    if (request->hasParam(_shelf10TemperatureNotification))
    {
      int temperatureNotification = request->getParam(_shelf10TemperatureNotification)->value().toInt();
      _settings.shelf10TemperatureNotification = temperatureNotification;
      _context.shelf10TemperatureNotificationSent = false;
    }
    if (request->hasParam(_headerTemperatureNotification))
    {
      int temperatureNotification = request->getParam(_headerTemperatureNotification)->value().toInt();
      _settings.headerTemperatureNotification = temperatureNotification;
      _context.headerTemperatureNotificationSent = false;
    }
    if (request->hasParam(_tankTemperatureNotification))
    {
      int temperatureNotification = request->getParam(_tankTemperatureNotification)->value().toInt();
      _settings.tankTemperatureNotification = temperatureNotification;
      _context.tankTemperatureNotificationSent = false;
    }
    if (request->hasParam(_waterTemperatureNotification))
    {
      int temperatureNotification = request->getParam(_waterTemperatureNotification)->value().toInt();
      _settings.waterTemperatureNotification = temperatureNotification;
      _context.waterTemperatureNotificationSent = false;
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
    if (request->hasParam(_heater2))
    {
      int state = request->getParam(_heater2)->value().toInt();
      _settings.heater2State = state > 0 ? HIGH : LOW;
      _settings.heater2StateChanged = true;
    }
    if (request->hasParam(_heater3))
    {
      int state = request->getParam(_heater3)->value().toInt();
      _settings.heater3State = state > 0 ? HIGH : LOW;
      _settings.heater3StateChanged = true;
    }
    if (request->hasParam(_tankSize))
    {
      int tankSize = request->getParam(_tankSize)->value().toInt();
      if (tankSize <= 500 && tankSize >= 0)
      {
        _settings.tankSize = tankSize;
      }
    }
    if (request->hasParam(_csvTimeFrameInSeconds))
    {
      int csvTimeFrame = request->getParam(_csvTimeFrameInSeconds)->value().toInt();
      _settings.csvTimeFrameInSeconds = csvTimeFrame;
    }
    if (request->hasParam(_pushNotificationCode))
    {
      String notificationCode = request->getParam(_pushNotificationCode)->value();
      strlcpy(_settings.pushNotificationCode, notificationCode.c_str(), sizeof(_settings.pushNotificationCode));
    }
    if (request->hasParam(_tempOfTheDay))
    {
      auto tempOfTheDay = request->getParam(_tempOfTheDay)->value().toFloat();
      _context.tempOfTheDay = tempOfTheDay;
      _context.tempofTheDayNotificationTime = millis();
    }
    if (request->hasParam(_tempOfTheDayNotificationDelayInSeconds))
    {
      auto tempOfTheDayDelay = request->getParam(_tempOfTheDayNotificationDelayInSeconds)->value().toInt();
      _context.tempOfTheDayNotificationDelayInSeconds = tempOfTheDayDelay;
    }
    if (request->hasParam(_tempOfTheDayDeviation))
    {
      auto tempOfTheDayDeviation = request->getParam(_tempOfTheDayDeviation)->value().toFloat();
      _context.tempOfTheDayDeviation = tempOfTheDayDeviation;
    }
    if (request->hasParam(_wifiSsid))
    {
      auto wifiSsid = request->getParam(_wifiSsid)->value();
      _settings.wifiSsid = wifiSsid;
    }
    if (request->hasParam(_wifiPassword))
    {
      auto wifiPassword = request->getParam(_wifiPassword)->value();
      _settings.wifiPassword = wifiPassword;
    }

    _configurationService.saveConfiguration();

    request->send(200, "text/html");
    return;
  });
}