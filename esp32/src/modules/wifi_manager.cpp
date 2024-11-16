// wifi_manager.cpp
#include "wifi_manager.h"

void WifiManager::init()
{
    // Setup AP
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());

    // Setup Station
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void WifiManager::update()
{
    // Reconnect if necessary
    if (WiFi.status() != WL_CONNECTED)
    {
        WiFi.reconnect();
        Serial.print("Station IP address: ");
        Serial.println(WiFi.localIP());
    }
}
