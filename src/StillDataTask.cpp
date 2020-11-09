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

    char csvEntry[100];
    sprintf(csvEntry, "%i;%.2f;%.2f;%.2f;%.2f;%i; %\n", (int)(millis()/1000) ,(float)_data.shelf10, (float)_data.header, (float)_data.tank, (float)_data.water, (int)_settings.percentagePower);
    _fileService.saveFile(_fileName, csvEntry);
}

uint32_t StillDataTaskClass::timeOfNextCheck()
{
    setTriggered(true);
    return millisToMicros(_settings.csvTimeFrameInSeconds * 1000);
}
