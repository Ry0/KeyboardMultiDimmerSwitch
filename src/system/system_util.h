#ifndef _SYSTEM_UTIL_H_
#define _SYSTEM_UTIL_H_

#include <WiFi.h>
#include <FS.h>
#include <SPIFFS.h>
#include "../data_struct.h"

class system_util
{
public:
    system_util();

    void connect_wifi(const char *ssid, const char *passphrase);
    void reboot();
    void reboot(unsigned long period_sec);


private:
};

#endif
