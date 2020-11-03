#ifndef __DATACONTEXT_H__
#define __DATACONTEXT_H__
#include <Arduino.h>
#include <DallasTemperature.h>

class StillDataContextClass
{
public:
    StillDataContextClass(DeviceAddress &_shelv10Address, DeviceAddress &_headAddress, DeviceAddress &_tankAddress, DeviceAddress &_waterAddress,
                          String _shelv10Name, String _headName, String _tankName, String _waterName);
    DeviceAddress& shelv10Address;
    DeviceAddress& headAddress;
    DeviceAddress& tankAddress;
    DeviceAddress& waterAddress;
    String shelv10Name;
    String headName;
    String tankName;
    String waterName;

    bool findSensors = false;
    DeviceAddress foundDevices[4] = {};
    String foundDeviceString[4] = {};
};


#endif // __DATACONTEXT_H__