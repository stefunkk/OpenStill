#ifndef __DATACONTEXT_H__
#define __DATACONTEXT_H__
#include <Arduino.h>
#include <DallasTemperature.h>

class StillDataContextClass
{
public:
    StillDataContextClass(DeviceAddress &_shelf10Address, DeviceAddress &_headAddress, DeviceAddress &_tankAddress, DeviceAddress &_waterAddress,
                          String _shelf10Name, String _headName, String _tankName, String _waterName);
    DeviceAddress& shelf10Address;
    DeviceAddress& headAddress;
    DeviceAddress& tankAddress;
    DeviceAddress& waterAddress;
    String shelf10Name;
    String headName;
    String tankName;
    String waterName;

    bool findSensors = false;
    bool clearCsv = false;
    DeviceAddress foundDevices[4] = {};
    String foundDeviceString[4] = {};
};


#endif // __DATACONTEXT_H__