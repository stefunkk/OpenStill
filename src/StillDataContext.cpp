#include "StillDataContext.h"
StillDataContextClass::StillDataContextClass(DeviceAddress &_shelf10Address, DeviceAddress &_headAddress, DeviceAddress &_tankAddress, DeviceAddress &_waterAddress,
                                             String _shelf10Name, String _headName, String _tankName, String _waterName) :
                                              shelf10Address(_shelf10Address), headAddress(_headAddress), tankAddress(_tankAddress), 
                                              waterAddress(_waterAddress), shelf10Name(_shelf10Name), headName(_headName), 
                                              tankName(_tankName), waterName(_waterName), notifications({}), tempofTheDayNotificationTime(millis())
{
}

