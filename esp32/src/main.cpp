#include "config.h"
#include "modules/wifi_manager.h"
#include "modules/data_decoder.h"
#include "modules/gpio_controller.h"
#include "modules/web_server.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_task_wdt.h"

// Task Handle
TaskHandle_t gpioTaskHandle = NULL;

esp_err_t hadle = ESP_ERR_INVALID_STATE;

// GPIO Update Task Function
void gpioUpdateTask(void *pvParameters)
{
    // This function will run in its own thread
    esp_task_wdt_init(10, false); // 30 seconds timeout

    hadle = esp_task_wdt_add(gpioTaskHandle);

    while (true)
    {
        GpioController::update(); // Call the GPIO update in its own thread

        esp_task_wdt_reset();
        /* Serial.println("Task running ex, watchdog reset."); */
    }
}

void setup()
{
    // Start serial communication
    Serial.begin(921600);

    delay(100);

    // Initialize Wi-Fi (optional, commented out)
    // WifiManager::init();

    // Initialize modules
    DataDecoder::init();
    GpioController::init();

    // Create the GPIO update task (this runs in its own thread)
    xTaskCreate(
        gpioUpdateTask,     // Function to implement the task
        "GPIO Update Task", // Name of the task
        2048,               // Stack size (increase if necessary)
        NULL,               // Parameters to pass to the task
        2,                  // Priority (1 is low priority)
        &gpioTaskHandle     // Task handle to keep track of the task
    );

    // Initialize Web Server (optional, commented out)
    // WebServer::init();
}

void loop()
{
    // Check for serial data and update the DataDecoder
    if (Serial.available())
    {
        DataDecoder::update(Serial.read());
    }

    // The GPIO task is now running in the background, so nothing else is needed here
    // You can put other tasks here (like WebServer::update(), if enabled)
    // WebServer::update();  // Uncomment if using the web server
}
