// gpio_controller.cpp
#include "gpio_controller.h"

void GpioController::init()
{
    pinMode(DATA_PIN, OUTPUT);
    pinMode(CLOCK_PIN_ANODE, OUTPUT);
    pinMode(CLOCK_PIN_CATHODE, OUTPUT);
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(OE_PIN, OUTPUT);
}

void __SOB(uint8_t dataPin, uint8_t clockPin, uint8_t val)
{
    digitalWrite(dataPin, val);
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
}

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

void GpioController::update()
{
    uint32_t cathode_pattern;
    //printBits32(cathode_pattern);
    // Apply bit shifting or any required operation to GPIO pins based on data_array
    __SOB(DATA_PIN, CLOCK_PIN_ANODE, 1); // initial bit
    digitalWrite(LATCH_PIN, LOW);
    for (uint8_t i = 0; i < 64; i++)
    {
        cathode_pattern = (DataDecoder::data_array[i]);
        //printBits32(cathode_pattern);

        digitalWrite(OE_PIN, HIGH);

        for (uint8_t i = 0; i < 4; i++)
        {
            shiftOut(DATA_PIN, CLOCK_PIN_CATHODE, LSBFIRST, (cathode_pattern >> (i * 8)));
        }

        digitalWrite(LATCH_PIN, HIGH);
        digitalWrite(LATCH_PIN, LOW);

        __SOB(DATA_PIN, CLOCK_PIN_ANODE, 0); // shifting down the rows

        digitalWrite(OE_PIN, LOW);
        delayMicroseconds(400);
    }
}
