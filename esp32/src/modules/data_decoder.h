// data_decoder.h
#ifndef DATA_DECODER_H
#define DATA_DECODER_H

#include <Arduino.h>

class DataDecoder {
public:
    static void init();
    static void update();
    static uint32_t data_array[ARRAY_SIZE];
};

#endif // DATA_DECODER_H
