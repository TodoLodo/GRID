// wifi_manager.h
#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include "config.h"

class WifiManager {
public:
    static void init();
    static void update();
};

#endif // WIFI_MANAGER_H
