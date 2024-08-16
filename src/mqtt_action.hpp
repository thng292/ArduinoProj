#pragma once
#include <PubSubClient.h>

#include <string_view>

#include "utils.hpp"

namespace AA::MQTT_ACTION {
    using sv = std::string_view;

    constexpr struct {
        sv write_feed_time = "write_feed_time";
        sv feed_now = "feed_now";
        sv restart = "restart";
        sv get_dev_info = "get_dev_info";
        sv call = "call";
        sv sensor_state = "sensor_state"; //
        sv add_image = "add_image"; //
        sv add_video = "add_video"; //
        sv request_feed_time = "request_feed_time"; //
        sv log = "log"; //
        sv dev_info = "dev_info"; //
        sv time_eat = "time_eat"; //
        sv water_added = "water_added"; //
    } TOPICS;
    constexpr uint8_t NUM_SUBSCRIBE_TOPICS = 5;

    auto sensor_state(PubSubClient& mqtt_client, SensorsState state) -> void;

    auto add_image(PubSubClient& mqtt_client, const std::string& image_link)
        -> void;

    auto add_video(PubSubClient& mqtt_client, const std::string& image_link)
        -> void;

    auto dev_info(PubSubClient& mqtt_client) -> void;

    auto time_eat(PubSubClient& mqtt_client, unsigned long time_eat_ms) -> void;

    auto water_added(PubSubClient& mqtt_client, unsigned long gram_water_added)
        -> void;

    auto push_log(PubSubClient& mqtt_client, const std::string& log) -> void;

    auto request_feed_time(PubSubClient& mqtt_client) -> void;
}  // namespace AA::MQTT_ACTION