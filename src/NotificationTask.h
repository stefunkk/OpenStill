#ifndef __NOTIFICATIONTASK_H__
#define __NOTIFICATIONTASK_H__

#include "StillDataContext.h"
#include "Settings.h"
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <TaskManagerIO.h>

class NotificationTaskClass : public BaseEvent
{
public:
    NotificationTaskClass(StillDataContextClass &context, SettingsClass &settings);
    void exec() override;
    uint32_t timeOfNextCheck() override;

private:
    StillDataContextClass &_context;
    SettingsClass &_settings;
    void sendNotification(String title, String message);
};
#endif // __NOTIFICATIONTASK_H__