// data_decoder.cpp
#include "data_decoder.h"
#include <WiFi.h>
#include "config.h"

uint32_t DataDecoder::data_array[ARRAY_SIZE] = {0};

void DataDecoder::init()
{
    Serial.begin(115200);
}

void DataDecoder::update()
{
    static uint8_t col = 0, row = 0;

    uint8_t rec_byte = Serial.read();

    switch (rec_byte >> 6)
    {
    case 3:
        row = 0;
        col = 0;
        break;

    case 1:
        row++;
        col = 0;
        break;

    default:
        break;
    }

    DataDecoder::data_array[row] &= (2 ^ 6 - 1) << col;
    DataDecoder::data_array[row] |= (uint32_t)(recByte & (2 ^ 6 - 1)) << col;

    col += 6;
}
