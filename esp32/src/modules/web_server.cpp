// web_server.cpp
#include "web_server.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "config.h"

AsyncWebServer server(80);

void WebServer::init() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", "<h1>ESP32 Control Interface</h1>");
    });
    server.begin();
}

void WebServer::update() {
    // Web server runs in background
}
