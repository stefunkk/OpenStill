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
  const char *host = "www.pushsafer.com";
  const uint16_t port = 443;
  String path = "/api";
  Serial.println("Sending notification: " + message);

  WiFiClientSecure client;
  client.setInsecure();


  HTTPClient https;

  if (https.begin(client, host, port, path))
  {
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpsCode = https.POST("k=" + String(_settings.pushNotificationCode) + "&t=" + title + "&m=" + message + "&d=a&u=http://" + _context.ipAddress);

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
