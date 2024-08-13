#include "mqtt_action.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdio>

#include "utils.hpp"

auto AA::sensor_state(PubSubClient& mqtt_client, SensorsState state) -> void
{
    auto buffer = std::array<char, 128>{0};
    auto len = snprintf(
        buffer.data(),
        buffer.max_size(),
        "{\"temp\": %f, \"humid\": %f, \"food\": %f, \"water\": %f, }",
        state.temp,
        state.humid,
        state.food,
        state.water
    );
    mqtt_client.publish("topic/device_state", buffer.data(), len);
}