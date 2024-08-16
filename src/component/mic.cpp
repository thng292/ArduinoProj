#include "mic.hpp"

#include <Esp.h>

#include <cstdlib>

auto AA::Mic::read() -> uint16_t
{
    return random(0, 4095);
}

auto AA::Mic::begin() -> void { pinMode(this->pin, INPUT); }
