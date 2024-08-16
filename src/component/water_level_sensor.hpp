#pragma once
#include <Esp.h>

#include <cstdint>

namespace AA {
    class WaterLevelSensor {
        uint8_t pin = 0;
        constexpr static uint16_t ESP_ANALOG_RES = 4095;

       public:
        WaterLevelSensor(uint8_t pin) : pin(pin) {}

        auto begin() -> void
        {
            pinMode(this->pin, INPUT);
        }

        [[nodiscard]] auto read() const noexcept -> float
        {
            return (float)analogRead(this->pin) / ESP_ANALOG_RES;
        }
    };
}  // namespace AA