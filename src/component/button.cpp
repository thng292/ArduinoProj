#include "button.hpp"
#include <Esp.h>
#include <cstdint>

auto AA::Button::begin() -> void
{
    pinMode(this->pin, INPUT);
}

auto AA::Button::isPressed() -> bool
{
    return static_cast<bool>(digitalRead(this->pin));
}

auto AA::Button::read() -> uint8_t
{
    return static_cast<uint8_t>(digitalRead(this->pin));
}