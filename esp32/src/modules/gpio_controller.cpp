// gpio_controller.cpp
#include "gpio_controller.h"
#include "data_decoder.h"
#include "config.h"

void GpioController::init()
{
    pinMode(DATA_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(LATCH_PIN_ANODE, OUTPUT);
    pinMode(LATCH_PIN_CATHODE, OUTPUT);
    pinMode(OE_PIN, OUTPUT);
}

void GpioController::update()
{
    uingt64_t anode_pattern;
    uint32_t cathode_patter;
    // Apply bit shifting or any required operation to GPIO pins based on data_array
    for (i = 0; i < 64; i++)
    {
        anode_pattern = 1llu << i;
        cathode_pattern = ~(DataDecoder::data_array[i]);

        digitalWrite(OE_PIN, HIGH);

        digitalWrite(LATCH_PIN_ANODE, LOW); // Enable the main shift register to set the 64-bit first byte pattern
        // Shift out the 64-bit first byte pattern (8 bytes, or 64 bits)
        for (uint8_t i = 0; i < 8; i++)
        {
            shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, (firstbytepattern >> (i * 8)));
        }
        digitalWrite(LATCH_PIN_ANODE, HIGH);

        // Enable the secondary shift register to set the full 32-bit second byte pattern
        digitalWrite(LATCH_PIN_CATHODE, LOW);
        for (uint8_t i = 0; i < 4; i++)
        {
            shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, (firstbytepattern >> (i * 8)));
        }
        digitalWrite(LATCH_PIN_CATHODE, HIGH);

        digitalWrite(OE_PIN, LOW);
    }
}
