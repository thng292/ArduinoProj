#include "camera.hpp"

#include <Esp.h>

#include <array>
#include <string_view>

#define IMG_PRE "https://unsplash.com/photos/"
#define IMG_POST "/download?force=true&w=640"

#define VIDEO_PRE \
    "http://commondatastorage.googleapis.com/gtv-videos-bucket/sample/"

auto getDistance(uint8_t trig_pin, uint8_t echo_pin) -> float
{
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);
    auto duration = pulseIn(echo_pin, HIGH);
    auto distanceCm = duration * 0.034f / 2;
    return distanceCm;
}

auto AA::Camera::begin() -> void
{
    pinMode(this->pins.echo, INPUT);
    pinMode(this->pins.trig, OUTPUT);
    this->camera_thread = std::thread([this]() {
        while (true) {
            this->last_distance = getDistance(this->pins.trig, this->pins.echo);
            delay(100);
        }
    });
}

auto AA::Camera::startRecord() -> void
{
    this->is_recording = true;
}

auto AA::Camera::isRecording() -> boolean
{
    return this->is_recording;
}

// auto AA::Camera::captureAndUpload() -> std::string
// {
//     constexpr auto images = std::array<std::string_view, 6>{
//         IMG_PRE "IFxjDdqK_0U" IMG_POST,
//         IMG_PRE "9UUoGaaHtNE" IMG_POST,
//         IMG_PRE "w2DsS-ZAP4U" IMG_POST,
//         IMG_PRE "gKXKBY-C-Dk" IMG_POST,
//         IMG_PRE "cWOzOnSoh6Q" IMG_POST,
//         IMG_PRE "LcAZcVWsCIo" IMG_POST,
//     };
//     cursor = cursor % images.size();
//     return std::string(images[cursor++]);
// }

auto AA::Camera::stopRecordAndUpload() -> std::string
{
    this->is_recording = false;
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

auto AA::Camera::loop() -> void {}