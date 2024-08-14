#include "camera.hpp"

#include <array>
#include <string_view>

#include <Esp.h>

#define IMG_PRE "https://unsplash.com/photos/"
#define IMG_POST "/download?force=true&w=640"

#define VIDEO_PRE \
    "http://commondatastorage.googleapis.com/gtv-videos-bucket/sample/"

auto AA::Camera::begin() -> void
{
    pinMode(this->pins.echo, INPUT);
    pinMode(this->pins.trig, OUTPUT);
}

auto AA::Camera::captureAndUpload() -> std::string
{
    constexpr auto images = std::array<std::string_view, 6>{
        IMG_PRE "IFxjDdqK_0U" IMG_POST,
        IMG_PRE "9UUoGaaHtNE" IMG_POST,
        IMG_PRE "w2DsS-ZAP4U" IMG_POST,
        IMG_PRE "gKXKBY-C-Dk" IMG_POST,
        IMG_PRE "cWOzOnSoh6Q" IMG_POST,
        IMG_PRE "LcAZcVWsCIo" IMG_POST,
    };
    cursor = cursor % images.size();
    return std::string(images[cursor++]);
}

auto AA::Camera::recordAndUpload() -> std::string
{
    constexpr auto videos = std::array<std::string_view, 6>{
        VIDEO_PRE "BigBuckBunny.mp4",
        VIDEO_PRE "ElephantsDream.mp4",
        VIDEO_PRE "ForBiggerBlazes.mp4",
        VIDEO_PRE "ForBiggerEscapes.mp4",
        VIDEO_PRE "ForBiggerFun.mp4",
        VIDEO_PRE "ForBiggerJoyrides.mp4",
    };
    cursor = cursor % videos.size();
    return std::string(videos[cursor++]);
}

auto AA::Camera::isPetIn() const noexcept -> bool
{
    return this->last_distance < 50.0f;
}

auto AA::Camera::loop() -> void
{
    auto time = millis();
    switch (this->current_stage) {
        case Stage::Reset:
            if (time - this->last_time_ms >= 2) {
                this->current_stage = Stage::Echo;
                last_time_ms = time;
            } else {
                digitalWrite(this->pins.trig, LOW);
            }
            break;
        case Stage::Echo:
            if (time - this->last_time_ms >= 10) {
                digitalWrite(this->pins.trig, LOW);
                auto duration_ms = pulseIn(this->pins.echo, HIGH);
                this->last_distance = duration_ms * 0.034f / 2.0f;
                this->last_time_ms = millis();
                this->current_stage = Stage::Reset;
            } else {
                digitalWrite(this->pins.trig, HIGH);
            }
            break;
    }
}