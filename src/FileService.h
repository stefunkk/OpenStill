#ifndef __FILESERVICE_H__
#define __FILESERVICE_H__

#include <Arduino.h>
#include <FS.h>

class FileServiceClass
{
public:
    FileServiceClass();
    String openFile(String path);
    void saveFile(String path, String content);
    void removeFile(String path);
private:
    FS& _fs;
    bool _fileLock = false;
};

#endif // __FILESERVICE_H__