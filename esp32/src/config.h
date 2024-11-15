#ifndef CONFIG_H
#define CONFIG_H

// Wi-Fi credentials for station mode
#ifndef WIFI_SSID
#define WIFI_SSID "YourSSID"
#endif
#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "YourPassword"
#endif

// IP settings
#define AP_SSID "ESP32_AP"
#define AP_PASSWORD "12345678"

// GPIO pin definitions
#define DATA_PIN 4
#define CLOCK_PIN_ANODE 12
#define CLOCK_PIN_CATHODE 13
#define LATCH_PIN 14
#define OE_PIN 2

// Array size
#define ARRAY_SIZE 64

#endif // CONFIG_H
