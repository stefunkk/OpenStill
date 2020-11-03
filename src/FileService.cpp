#include "FileService.h"

FileServiceClass::FileServiceClass(): _fs(LittleFS)
{
    LittleFS.begin();
}

String FileServiceClass::openFile(String path) 
{
    File file = _fs.open(path, "r");
    
    if (!file)
    {
        Serial.println(path);
        Serial.println(F("Failed to read configuration file - FS error or file missing."));
        return "";
    }

    auto result = file.readString();

    file.close();

    return result;
}

void FileServiceClass::saveFile(String path, String content) 
{
    
    File file = _fs.open(path, "w");
    if (!file)
    {
        Serial.println(F("Failed to create file"));
        return;
    }

    file.write(content);

    file.close();
}

void FileServiceClass::removeFile(String path) 
{
    _fs.remove(path);
}
