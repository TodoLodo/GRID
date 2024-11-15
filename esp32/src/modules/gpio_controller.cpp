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

void GpioController::update()
{
    uint32_t cathode_pattern;

    // Apply bit shifting or any required operation to GPIO pins based on data_array
    __SOB(DATA_PIN, CLOCK_PIN_ANODE, 1); // initial bit
    for (uint8_t i = 0; i < 64; i++)
    {
        cathode_pattern = ~(DataDecoder::data_array[i]);

        digitalWrite(OE_PIN, HIGH);

        for (uint8_t i = 0; i < 4; i++)
        {
            shiftOut(DATA_PIN, CLOCK_PIN_CATHODE, LSBFIRST, (cathode_pattern >> (i * 8)));
        }

        digitalWrite(LATCH_PIN, HIGH);
        digitalWrite(LATCH_PIN, LOW);

        __SOB(DATA_PIN, CLOCK_PIN_ANODE, 0); // shifting down the rows

        digitalWrite(OE_PIN, LOW);
    }
}