#include "NotificationHelper.h"

bool NotificationHelperClass::addNotification(StillDataContextClass& context, String name, String message) 
{
    for(int i = 0; i < 10; i++)
    {
        if (context.notifications[i].isSent){
            context.notifications[i].message = message;
            context.notifications[i].name = name;
            context.notifications[i].isSent = false;
            return true;
        }
    }

    return false;
}
