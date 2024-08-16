#include "mqtt_action.hpp"
#include <cstdio>
#include <memory>
#include <WiFi.h>

#include "utils.hpp"

constexpr size_t buffer_size = 512;
const auto buffer = std::make_unique<char[]>(buffer_size);

auto AA::MQTT_ACTION::sensor_state(PubSubClient& mqtt_client, SensorsState state) -> void
{
    auto len = snprintf(
        buffer.get(),
        buffer_size,
        "{\"temp\": %f, \"humid\": %f, \"food\": %f, \"water\": %f, }",
        state.temp,
        state.humid,
        state.food,
        state.water
    );
    mqtt_client.publish("device_state", buffer.get());
}

auto AA::MQTT_ACTION::add_image(
    PubSubClient& mqtt_client, const std::string& image_link
) -> void
{
    mqtt_client.publish("add_image", image_link.c_str());
}

auto AA::MQTT_ACTION::add_video(
    PubSubClient& mqtt_client, const std::string& image_link
) -> void
{
    mqtt_client.publish("add_video", image_link.c_str());
}

auto AA::MQTT_ACTION::dev_info(PubSubClient& mqtt_client) -> void
{
    auto ip = WiFi.localIP().toString();
    auto len = snprintf(
        buffer.get(),
        buffer_size,
        "{\"software\": %s,"
        "\"up_time\": %lu,"
        "\"chip\": %s,"
        "\"chip_revision\": %d,"
        "\"core\": %d,"
        "\"cpu_freq_mhz\": %d,"
        "\"free_heap\": %d,"
        "\"ip\": %s,"
        "\"wifi\": %s, }",
        "1.0.0",
        millis(),
        ESP.getChipModel(),
        ESP.getChipRevision(),
        ESP.getChipCores(),
        ESP.getCpuFreqMHz(),
        ESP.getFreeHeap(),
        ip.c_str(),
        WiFi.BSSID()
    );
    mqtt_client.publish("dev_info", buffer.get());
}

auto AA::MQTT_ACTION::time_eat(PubSubClient& mqtt_client, unsigned long time_eat_ms) -> void
{
    auto len = snprintf(buffer.get(), buffer_size, "%llu", time_eat_ms);
    mqtt_client.publish("time_eat", buffer.get());
}

auto AA::MQTT_ACTION::push_log(
    PubSubClient& mqtt_client, const std::string& log
) -> void
{
    mqtt_client.publish("log", log.c_str());
}