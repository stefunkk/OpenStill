#include "StillDataTask.h"

StillDataTaskClass::StillDataTaskClass(StillDataContextClass &context, FileServiceClass &fileService, SensorDataClass &data, SettingsClass &settings) : _settings(settings), _data(data), _fileService(fileService), _context(context)
{
}

void StillDataTaskClass::exec()
{
    if (_context.clearCsv)
    {
        _fileService.removeFile(_fileName);
        _context.clearCsv = false;
    }

    char csvEntry[300];
    sprintf(csvEntry, "%f;%f;%f;%f;%i%\n", _data.shelf10, _data.header, _data.tank, _data.water, _settings.percentagePower);
    _fileService.saveFile(_fileName, csvEntry);
}

uint32_t StillDataTaskClass::timeOfNextCheck()
{
    setTriggered(true);
    return millisToMicros(5000);
}
