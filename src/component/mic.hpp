#pragma once
#include <cstdint>

namespace AA {
    struct Buffer {
        uint8_t* data = nullptr;
        std::size_t size = 0;
    };

    class Mic {
        uint8_t pin = 0;
        std::size_t cursor = 0;

       public:
        Mic(uint8_t pin) : pin(pin) {};

        auto begin() -> void;

        auto read(Buffer buffer) -> void;
    };
}  // namespace AA