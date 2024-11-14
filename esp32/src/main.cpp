#include "config.h"
#include "modules/wifi_manager.h"
#include "modules/data_decoder.h"
#include "modules/gpio_controller.h"
#include "modules/web_server.h"

void setup()
{
    // Initialize Wi-Fi
    /* WifiManager::init(); */

    // Initialize modules
    DataDecoder::init();
    GpioController::init();
    /* WebServer::init(); */
}

void loop()
{
    // Update modules
    /* WifiManager::update(); */
    if (Serial.available())
    {
        DataDecoder::update();
    }
    GpioController::update();
    /* WebServer::update(); */

    delay(10); // Small delay to prevent watchdog resets
}
