#ifndef __NOTIFICATIONTASK_H__
#define __NOTIFICATIONTASK_H__

#include "StillDataContext.h"
#include "Settings.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <TaskManagerIO.h>

class NotificationTaskClass : public BaseEvent
{
public:
    NotificationTaskClass(StillDataContextClass &context, SettingsClass &settings);
    void exec() override;
    uint32_t timeOfNextCheck() override;

private:
    SettingsClass &_settings;
    StillDataContextClass &_context;
    void sendNotification(String title, String message);
};
#endif // __NOTIFICATIONTASK_H__