#pragma once
#include <cstdint>

namespace AA {
    class Button {
        uint8_t pin = 0;

       public:
        Button() = default;

        Button(uint8_t pin) : pin(pin) {}

        auto isPressed() -> bool;
        auto read() -> uint8_t;
    };
}  // namespace AA