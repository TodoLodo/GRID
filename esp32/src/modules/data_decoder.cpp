// data_decoder.cpp
#include "data_decoder.h"
#include <driver/timer.h>

volatile uint32_t DataDecoder::data_array[ARRAY_SIZE] = {0};
uint8_t DataDecoder::frame_ready = 0;

void DataDecoder::init()
{
    /* for (int i = 0; i < ARRAY_SIZE; i++)
    {
        if (i % 2 == 0)
        {
            DataDecoder::data_array[i] = 0xFFFFFFFF; // Set even-indexed rows to 1s
        }
        else
        {
            DataDecoder::data_array[i] = 0x00000000; // Set odd-indexed rows to 0s
        }
    } */
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
    static uint8_t col = 0, row = 0, payload = 0, fms_state = 0;
    static uint32_t start = 0;

    /* Serial.print("Recieved : ");
    Serial.print(rec_byte);
    Serial.print(", fms: ");
    Serial.println(fms_state); */
    Serial.printf("byte rate: %luus\n", micros() - start);
    start = micros();

    if (rec_byte == 255)
    {
        fms_state = 1;
    }
    else if (fms_state == 4)
    {
        /* Serial.println(row);
        Serial.println(col);
        Serial.println(payload); */
        if (rec_byte == uint8_t(((row ^ col ^ payload) | 0x80) & 0xfe))
        {
            uint8_t mask = ((1U << 7) - 1); // 7-bit mask
            uint8_t shift_by = (63 - 6) - col;
            DataDecoder::data_array[row] &= ~(((uint64_t)mask << shift_by) >> 32);
            DataDecoder::data_array[row] |= ((uint64_t)(payload & mask) << shift_by) >> 32;

            /* Serial.print("row[");
            Serial.print(row);
            Serial.print("]: ");
            printBits32(DataDecoder::data_array[row]); */
        }

        fms_state = 0;

        /* if (row == 63 && col == 28)
        {
            Serial.print("frame recieved : ");
            Serial.print((uint64_t)micros() - start);
            Serial.println("ms");
        } */
    }
    else
    {
        switch (fms_state)
        {
        case 1:
            row = rec_byte;
            break;

        case 2:
            col = rec_byte;
            /* if (row == 0 && col == 0)
            {
                start = (uint64_t)micros();
            } */
            break;

        case 3:
            payload = rec_byte;
            break;

        default:
            break;
        }

        fms_state++;
    }

    esp_rom_delay_us(10);
}
