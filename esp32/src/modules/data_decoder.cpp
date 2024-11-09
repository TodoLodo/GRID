// data_decoder.cpp
#include "data_decoder.h"
#include <WiFi.h>
#include "config.h"

uint32_t DataDecoder::data_array[ARRAY_SIZE] = {0};

void DataDecoder::init() {
    Serial.begin(115200);
}

void DataDecoder::update() {
    if (Serial.available()) {
        // Priority 1: USB serial data processing
        for (int i = 0; i < ARRAY_SIZE; i++) {
            DataDecoder::data_array[i] = Serial.parseInt();
        }
    } else {
        // TODO: Handle socket communication and data reception
        // Use sockets or WiFi to check for data and update data_array as needed
    }
}
