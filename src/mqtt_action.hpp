#pragma once
#include <PubSubClient.h>

#include "utils.hpp"

namespace AA::MQTT_ACTION {
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
}  // namespace AA::MQTT_ACTION