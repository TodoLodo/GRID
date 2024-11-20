// wifi_manager.cpp
#include "wifi_manager.h"

IPAddress WifiManager::STA_IP = IPAddress(0, 0, 0, 0);
IPAddress WifiManager::AP_IP = IPAddress(1, 1, 1, 2);
IPAddress WifiManager::GATEWAY = IPAddress(1, 1, 1, 1);
IPAddress WifiManager::SUBNET = IPAddress(255, 255, 255, 0);

void WifiManager::init()
{
    // Setup AP
    WiFi.softAPConfig(WifiManager::AP_IP, WifiManager::GATEWAY, WifiManager::SUBNET);
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());

    // Setup Station
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    WiFi.setAutoConnect(1);
    
    uint8_t counter = 0;
    while (WiFi.status() != WL_CONNECTED && counter < 10)
    {
        delay(500);
        Serial.print(".");
        counter++;
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.print("Station IP address: ");
        WifiManager::STA_IP = WiFi.localIP();
        Serial.println(WifiManager::STA_IP);
    }
}

void WifiManager::update()
{
    // Reconnect if necessary
    if (WiFi.status() != WL_CONNECTED)
    {
        WiFi.reconnect();
        Serial.print("Reconnecting => SSID: ");
        Serial.print(WiFi.SSID());
        Serial.print(", PASS: ");
        Serial.println(WiFi.psk());
    }

    if (WiFi.isConnected())
    {
        WifiManager::STA_IP = WiFi.localIP();
    }

    /* Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("Station IP address: ");
    Serial.println(WiFi.localIP()); */
}
