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
    uint32_t start, now;

    //  Apply bit shifting or any required operation to GPIO pins based on data_array
    __SOB(DATA_PIN, CLOCK_PIN_ANODE, 1); // initial bit
    digitalWrite(LATCH_PIN, LOW);
    for (uint8_t i = 0; i < 64; i++)
    {
        start = micros();
        cathode_pattern = ~(DataDecoder::data_array[i]);
        // printBits32(cathode_pattern);

        digitalWrite(OE_PIN, HIGH);

        for (uint8_t j = 0; j < 4; j++)
        {
            shiftOut(DATA_PIN, CLOCK_PIN_CATHODE, LSBFIRST, (cathode_pattern >> (j * 8)));
        }

        digitalWrite(LATCH_PIN, HIGH);
        digitalWrite(LATCH_PIN, LOW);

        digitalWrite(OE_PIN, LOW);

        __SOB(DATA_PIN, CLOCK_PIN_ANODE, 0); // shifting down the rows

        esp_task_wdt_reset(); // Reset the watchdog timer to prevent task restart

        vTaskDelay(pdMS_TO_TICKS(1));
        //delayMicroseconds(400);

        // Microsecond delay
        //delayMicroseconds(std::max(0, 400 - (int)(micros() - start)));
        // Serial.println(std::max(0, 400 - (int)(micros() - start)));
    }
}
