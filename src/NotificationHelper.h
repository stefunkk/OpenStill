#ifndef __NOTIFICATIONHELPER_H__
#define __NOTIFICATIONHELPER_H__

#include "StillDataContext.h"

class NotificationHelperClass
{
public:
    static bool addNotification(StillDataContextClass &context, String name, String message);
};
#endif // __NOTIFICATIONHELPER_H__