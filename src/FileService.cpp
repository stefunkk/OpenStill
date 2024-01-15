#include "FileService.h"

FileServiceClass::FileServiceClass() : _fs(SPIFFS)
{
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
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

        file.close();
        _fileLock = false;

        return "";
    }

    auto result = file.readString();

    file.close();
    
    _fileLock = false;

    return result;
}

bool FileServiceClass::fileExists(String path)
{
    while (_fileLock)
    {
    }

    _fileLock = true;

    File file = _fs.open(path, "r");

    if (!file || file.size() == 0)
    {
        Serial.println(path);
        Serial.println(F("Failed to read file - FS error or file missing."));

         file.close();
        _fileLock = false;

        return false;
    }

    file.close();
    
    _fileLock = false;

    return true;
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
