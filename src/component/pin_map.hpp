#pragma once
#include <Arduino.h>
#include <sys/types.h>

#include <array>
#include <cstdint>

#include "HardwareSerial.h"

namespace AA::Pin {
    constexpr uint8_t BUILTIN_LED = 2;
    constexpr uint8_t BUTTON = 32;
    constexpr uint8_t DTH22 = 13;
    constexpr uint8_t MIC = 27;
    constexpr uint8_t BUZZER = 14;

    constexpr std::array<uint8_t, 2> SERVO = {2, 15};

    struct SCALE_PINS {
        uint8_t SCK = 0;
        uint8_t DT = 0;

        constexpr SCALE_PINS(uint8_t SCK, uint8_t DT) : SCK(SCK), DT(DT) {}
    };

    constexpr std::array<SCALE_PINS, 2> SCALE = {
        SCALE_PINS(4, RX2), SCALE_PINS(TX2, 21)
    };

    struct DISTANCE_SENS_PINS {
        uint8_t Trig = 0;
        uint8_t Echo = 0;

        constexpr DISTANCE_SENS_PINS(uint8_t Trig, uint8_t Echo)
            : Trig(Trig), Echo(Echo)
        {
        }
    };

    constexpr auto DISTANCE_SENS = AA::Pin::DISTANCE_SENS_PINS(26, 25);

    constexpr uint8_t SDCARD = 5;
}  // namespace AA::Pin
;  // namespace AA