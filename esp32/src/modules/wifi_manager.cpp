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
    delay(100);
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.print("Station IP address: ");
        Serial.println(WiFi.localIP());
    }
}

void WifiManager::update()
{
    // Reconnect if necessary
    if (WiFi.status() != WL_CONNECTED)
    {
        WiFi.reconnect();
        Serial.print("SSID: ");
        Serial.print(WIFI_SSID);
        Serial.print(", PASS: ");
        Serial.print(WIFI_PASSWORD);
        Serial.print("Station IP address: ");
        Serial.println(WiFi.localIP());
    }
}
