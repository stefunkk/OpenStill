#ifndef __FLASHDATAHELPER_H__
#define __FLASHDATAHELPER_H__

#include <WiFi.h>
#include "FileService.h"
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <TaskManagerIO.h>

class FlashDataHelperClass
{
public:
    static void downloadContentFiles(FileServiceClass &fileService);
};
#endif // __FLASHDATAHELPER_H__