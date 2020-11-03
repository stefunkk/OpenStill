#ifndef __STILLDATATASK_H__
#define __STILLDATATASK_H__

#include <TaskManagerIO.h>
#include "SensorData.h"
#include "Settings.h"
#include "FileService.h"

class StillDataTaskClass
{
public:
    StillDataTaskClass(FileServiceClass& fileService, SensorDataClass& data, SettingsClass& settings);

private:
    SettingsClass &_settings;
    SensorDataClass &_data;
    String _fileName = "/data.csv";
    FileServiceClass& _fileService;
};


#endif // __STILLDATATASK_H__