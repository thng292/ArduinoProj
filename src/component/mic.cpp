#include "mic.hpp"
#include "mic_in.hpp"
#include <cstdlib>


auto AA::Mic::read(AA::Buffer buffer) -> void
{
    for (size_t i = 0; i < buffer.size; i++) {
        buffer.data[i] = output_wav[cursor];
        cursor = (cursor + 1) % output_wav_len;
    }
}