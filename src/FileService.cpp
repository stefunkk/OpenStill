#include "FileService.h"

FileServiceClass::FileServiceClass() : _fs(SPIFFS)
{
    _fs.begin();
}

String FileServiceClass::openFile(String path)
{
    while (_fileLock)
    {
    }

    _fileLock = true;

    File file = _fs.open(path, "r");

    if (!file)
    {
        Serial.println(path);
        Serial.println(F("Failed to read configuration file - FS error or file missing."));
        return "";
    }

    auto result = file.readString();

    file.close();
    
    _fileLock = false;

    return result;
}

void FileServiceClass::saveFile(String path, String content)
{
    while (_fileLock)
    {
    }

    _fileLock = true;

    File file = _fs.open(path, "a");
    if (!file)
    {
        Serial.println(F("Failed to create file"));
        return;
    }

    file.print(content);

    file.close();

    _fileLock = false;
}

void FileServiceClass::removeFile(String path)
{
    _fs.remove(path);
}
