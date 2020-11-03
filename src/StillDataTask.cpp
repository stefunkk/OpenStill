#include "StillDataTask.h"

StillDataTaskClass::StillDataTaskClass(FileServiceClass &fileService, SensorDataClass &data, SettingsClass &settings):
    _settings(settings), _data(data), _fileService(fileService)
{
}
