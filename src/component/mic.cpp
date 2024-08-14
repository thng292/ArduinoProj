#include "mic.hpp"

#include <Esp.h>

#include <cstdlib>

#include "mic_in.hpp"

auto AA::Mic::read(AA::Buffer buffer) -> void
{
    for (size_t i = 0; i < buffer.size; i++) {
        buffer.data[i] = output_wav[cursor];
        cursor = (cursor + 1) % output_wav_len;
    }
}

auto AA::Mic::begin() -> void { pinMode(this->pin, INPUT); }
