#include "NotificationTask.h"

NotificationTaskClass::NotificationTaskClass(StillDataContextClass &context, SettingsClass &settings) : _context(context), _settings(settings)
{
}

void NotificationTaskClass::exec()
{
  for (int i = 0; i < 10; i++)
  {
    if (!_context.notifications[i].isSent)
    {
      sendNotification(_context.notifications[i].name, _context.notifications[i].message);
      _context.notifications[i].isSent = true;
      return;
    }
  }
}

uint32_t NotificationTaskClass::timeOfNextCheck()
{
  setTriggered(true);
  return millisToMicros(10000);
}

void NotificationTaskClass::sendNotification(String title, String message)
{
  const char *host = "pushme.vercel.app";
  const uint16_t port = 443;
  String path = "/api/sendNotification";
  Serial.println("Sending notification: " + message);

  BearSSL::WiFiClientSecure client;
  client.setInsecure();
  HTTPClient https;

  if (https.begin(client, host, port, path))
  {
    https.addHeader("Content-Type", "application/json");
    int httpsCode = https.POST("{\"code\":\"" + String(_settings.pushNotificationCode) + "\",\"title\":\"" + title + "\",\"message\":\"" + message + "\"}");

    if (httpsCode > 0)
    {
      if (httpsCode == HTTP_CODE_OK)
      {
        Serial.println(https.getString());
      }
    }
    else
    {
      Serial.print("failed to POST");
    }
  }
  else
  {
    Serial.print("failed to connect to server");
  }
}
