// data_decoder.cpp
#include "data_decoder.h"

volatile uint32_t DataDecoder::data_array[ARRAY_SIZE] = {0};
uint8_t DataDecoder::frame_ready = 0;

void DataDecoder::init()
{
}

void printBits8(uint8_t value)
{
    for (int i = 7; i >= 0; i--)
    {
        // Extract the bit by shifting and masking
        uint8_t bit = (value >> i) & 1;
        Serial.print(bit);
    }
    Serial.print(" "); // New line after printing all bits
}

void _printBits32(uint32_t value)
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

void DataDecoder::update(uint8_t rec_byte)
{
    static uint8_t col = 0, row = 0;
    static u_int32_t last_row = 0;

    // Debugging before the update
    Serial.print("Before update row ");
    Serial.print(row);
    Serial.print(": ");
    Serial.println(DataDecoder::data_array[row], BIN);

    switch (rec_byte >> 6)
    {
    case 3:
        row = 0;
        col = 0;
        /* Serial.println("\nnew frame"); */
        break;

    case 1:
        row++;
        col = 0;
        break;

    default:
        break;
    }

    uint32_t mask = ((1U << 6) - 1); // 6-bit mask
    DataDecoder::data_array[row] &= ~(mask << col);
    DataDecoder::data_array[row] |= (rec_byte & mask) << col;

    col += 6;

    // Debugging after the update
    Serial.print("After update row ");
    Serial.print(row);
    Serial.print(": ");
    Serial.println(DataDecoder::data_array[row], BIN);
}
