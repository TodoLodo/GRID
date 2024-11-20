// wifi_manager.h
#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include "config.h"

class WifiManager {
public:
    static IPAddress STA_IP;
    static IPAddress AP_IP;
    static IPAddress GATEWAY;
    static IPAddress SUBNET;

    static void init();
    static void update();
};

#endif // WIFI_MANAGER_H
