// data_decoder.h
#ifndef DATA_DECODER_H
#define DATA_DECODER_H

#include <Arduino.h>
#include <cstdint>
#include <bitset>

#include "config.h"
#include "data_decoder.h"

class DataDecoder {
public:
    static void init();
    static void update(uint8_t rec_byte);
    static volatile uint32_t data_array[ARRAY_SIZE];
    static uint8_t frame_ready;
};

#endif // DATA_DECODER_H
