#include "config.h"
#include "modules/wifi_manager.h"
#include "modules/data_decoder.h"
#include "modules/gpio_controller.h"
#include "modules/web_server.h"

void setup()
{

    Serial.begin(115200);
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
        DataDecoder::update(Serial.read());
    }

    // Debugging: Print all rows in the shared data_array
    /* Serial.println("Data array from main:");
    for (size_t i = 0; i < ARRAY_SIZE; i++)
    {
        Serial.print("Row ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(DataDecoder::data_array[i], BIN);
    } */

    GpioController::update();

    /* WebServer::update(); */
}
