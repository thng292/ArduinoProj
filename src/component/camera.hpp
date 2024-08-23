#pragma once

#include <Esp.h>

#include <cmath>
#include <string>
#include <thread>

#include "pin_map.hpp"

namespace AA {
    using namespace std::string_view_literals;

    class Camera {
        uint8_t cursor = 0;
        Pin::DISTANCE_SENS_PINS pins;
        float last_distance = infinityf();
        unsigned long last_time_ms = 0;
        std::thread cameraThread;

       public:
        Camera(Pin::DISTANCE_SENS_PINS pins) : pins(pins) {}

        auto begin() -> void;

        auto captureAndUpload() -> std::string;

        auto recordAndUpload() -> std::string;

        auto loop() -> void;

        auto isPetIn() const noexcept -> bool;

        auto get_distance() const noexcept -> float
        {
            return this->last_distance;
        }
    };
}  // namespace AA