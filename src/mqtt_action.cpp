#include "mqtt_action.hpp"

#include <WiFi.h>

#include <cstdio>
#include <memory>

#include "utils.hpp"

constexpr size_t buffer_size = 512;
const auto buffer = std::make_unique<char[]>(buffer_size);

auto AA::MQTT_ACTION::sensor_state(
    PubSubClient& mqtt_client, SensorsState state
) -> void
{
    auto len = snprintf(
        buffer.get(),
        buffer_size,
        "{\"temp\": %f, \"humid\": %f, \"food\": %f, \"water\": %f }",
        state.temp,
        state.humid,
        state.food,
        state.water
    );
    Serial.println("Publishing sensor_state");
    mqtt_client.publish(TOPICS.sensor_state.data(), buffer.get());
    Serial.println("Done Publishing sensor_state");
}

auto AA::MQTT_ACTION::add_image(
    PubSubClient& mqtt_client, const std::string& image_link
) -> void
{
    Serial.println("Publishing add_image");
    mqtt_client.publish(TOPICS.add_image.data(), image_link.c_str());
    Serial.println("Done Publishing add_image");
}

auto AA::MQTT_ACTION::add_video(
    PubSubClient& mqtt_client, const std::string& video_link
) -> void
{
    Serial.println("Publishing add_video");
    mqtt_client.publish(TOPICS.add_video.data(), video_link.c_str());
    Serial.println("Done Publishing add_video");
}

auto AA::MQTT_ACTION::dev_info(PubSubClient& mqtt_client) -> void
{
    auto ip = WiFi.localIP().toString();
    auto len = snprintf(
        buffer.get(),
        buffer_size,
        "{\"software\": \"%s\","
        "\"up_time\": %lu,"
        "\"chip\": \"%s\","
        "\"chip_revision\": %d,"
        "\"core\": %d,"
        "\"cpu_freq_mhz\": %d,"
        "\"free_heap\": %d,"
        "\"ip\": \"%s\","
        "\"wifi\": \"%s\" }",
        "1.0.0",
        millis(),
        ESP.getChipModel(),
        ESP.getChipRevision(),
        ESP.getChipCores(),
        ESP.getCpuFreqMHz(),
        ESP.getFreeHeap(),
        ip.c_str(),
        WiFi.SSID().c_str()
    );
    Serial.println("Publishing dev_info");
    mqtt_client.publish(TOPICS.dev_info.data(), buffer.get());
    Serial.println("Done Publishing dev_info");
}

auto AA::MQTT_ACTION::time_eat(
    PubSubClient& mqtt_client, unsigned long time_eat_ms
) -> void
{
    auto len = snprintf(buffer.get(), buffer_size, "%lu", time_eat_ms / 1000);
    Serial.println("Publishing time_eat");
    mqtt_client.publish(TOPICS.time_eat.data(), buffer.get());
    Serial.println("Done Publishing time_eat");
}

auto AA::MQTT_ACTION::push_log(
    PubSubClient& mqtt_client, const std::string& log
) -> void
{
    Serial.println(log.data());
    Serial.println("Publishing log");
    mqtt_client.publish(TOPICS.log.data(), log.c_str());
    Serial.println("Done Publishing log");
}

auto AA::MQTT_ACTION::water_added(
    PubSubClient& mqtt_client, unsigned long gram_water_added
) -> void
{
    auto len = snprintf(buffer.get(), buffer_size, "%lu", gram_water_added);
    Serial.println("Publishing water_added");
    mqtt_client.publish(TOPICS.water_added.data(), buffer.get());
    Serial.println("Done Publishing water_added");
}

auto AA::MQTT_ACTION::request_feed_time(PubSubClient& mqtt_client) -> void
{
    Serial.println("Publishing request_feed_time");
    mqtt_client.publish(TOPICS.request_feed_time.data(), "1");
    Serial.println("Done Publishing request_feed_time");
}
