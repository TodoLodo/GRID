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
TaskHandle_t dataTaskHandle = NULL;

void printBits32(uint32_t value)
{
    for (int i = 31; i >= 0; i--)
    {
        // Extract the bit by shifting and masking
        uint32_t bit = (value >> i) & 1;
        Serial.print(bit);

        // Print a space every 8 bits for better readability
        if (i % 8 == 0)
        {
            Serial.print(" ");
        }
    }
    Serial.println(); // New line after printing all bits
}

// GPIO Update Task Function
void gpioUpdateTask(void *pvParameters)
{
    // This function will run in its own thread
    esp_task_wdt_init(10, false); // 30 seconds timeout

    esp_task_wdt_add(NULL);

    while (true)
    {
        /* GpioController::update(); // Call the GPIO update in its own thread */
        Serial.println("================================");
        for (size_t i = 0; i < 64; i++)
        {
            Serial.print(i);
            Serial.print(": ");
            printBits32(DataDecoder::data_array[i]);
            taskYIELD();
            esp_task_wdt_reset();
        }

        esp_task_wdt_reset();
    }
}

void dataUpdateTask(void *pvParameters)
{
    // This function will run in its own thread
    esp_task_wdt_init(10, false); // 30 seconds timeout

    esp_task_wdt_add(NULL);

    while (true)
    {
        if (Serial.available())
        {
            DataDecoder::update(Serial.read());
        }

        esp_task_wdt_reset();
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
    xTaskCreatePinnedToCore(
        gpioUpdateTask,     // Function to implement the task
        "GPIO Update Task", // Name of the task
        2048,               // Stack size
        NULL,               // Parameters
        1,                  // Priority
        &gpioTaskHandle,    // Task handle
        1                   // Pin to Core 1
    );

    // Create the data dec update task (this runs in its own thread)
    xTaskCreatePinnedToCore(
        dataUpdateTask,     // Function to implement the task
        "DATA Update Task", // Name of the task
        4096,               // Stack size
        NULL,               // Parameters
        1,                  // Priority
        &dataTaskHandle,    // Task handle
        0                   // Pin to Core 1
    );

    // Initialize Web Server (optional, commented out)
    // WebServer::init();
}

void loop()
{
    // Check for serial data and update the DataDecoder

    // The GPIO task is now running in the background, so nothing else is needed here
    // You can put other tasks here (like WebServer::update(), if enabled)
    // WebServer::update();  // Uncomment if using the web server
}
