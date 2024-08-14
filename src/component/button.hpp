#pragma once
#include <cstdint>

namespace AA {
    class Button {
        uint8_t pin = 0;

       public:
        Button(uint8_t pin) : pin(pin) {}

        auto begin() -> void;

        auto isPressed() -> bool;
        auto read() -> uint8_t;
    };
}  // namespace AA