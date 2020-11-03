#include "StillDataContext.h"
StillDataContextClass::StillDataContextClass(DeviceAddress &_shelv10Address, DeviceAddress &_headAddress, DeviceAddress &_tankAddress, DeviceAddress &_waterAddress,
                                             String _shelv10Name, String _headName, String _tankName, String _waterName) : shelv10Address(_shelv10Address), headAddress(_headAddress), tankAddress(_tankAddress), waterAddress(_waterAddress),
                                                                                                                       shelv10Name(_shelv10Name), headName(_headName), tankName(_tankName), waterName(_waterName)
{
}

