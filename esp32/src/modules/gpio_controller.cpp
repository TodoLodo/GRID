// gpio_controller.cpp
#include "gpio_controller.h"
#include "data_decoder.h"
#include "config.h"

void GpioController::init() {
    pinMode(GPIO_PIN_1, OUTPUT);
    pinMode(GPIO_PIN_2, OUTPUT);
}

void GpioController::update() {
    // Apply bit shifting or any required operation to GPIO pins based on data_array
    for (int i = 0; i < ARRAY_SIZE; i++) {
        digitalWrite(GPIO_PIN_1, (DataDecoder::data_array[i] & 0x01) ? HIGH : LOW);
        digitalWrite(GPIO_PIN_2, (DataDecoder::data_array[i] & 0x02) ? HIGH : LOW);
    }
}
