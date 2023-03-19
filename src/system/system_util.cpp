#include "system_util.h"

system_util::system_util()
{
}

void system_util::connect_wifi(const char *ssid, const char *passphrase)
{
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, passphrase);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void system_util::reboot()
{
    Serial.println("reboot.");
    esp_restart();
}

void system_util::reboot(unsigned long period_sec)
{
    while (1)
    {
        delay(period_sec * 1000);
        Serial.printf("reboot at regular intervals.\n");
        reboot();
    }
}