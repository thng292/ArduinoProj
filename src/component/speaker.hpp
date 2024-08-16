#pragma once
#include <cstdint>

namespace AA {
    class SpeakerClass {
        uint8_t pin = 0;
        bool is_playing = false;
        unsigned long last_time = 0;
        unsigned long duration = 0;
        int idx = 0;
        auto playNote() -> void;

       public:
        auto attach(uint8_t pin) -> void { this->pin = pin; }

        auto loop() -> void;

        auto play() -> void;
    };
}  // namespace AA

extern AA::SpeakerClass Speaker;