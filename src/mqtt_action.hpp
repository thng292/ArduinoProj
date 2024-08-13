#pragma once
#include <PubSubClient.h>
#include "utils.hpp"

namespace AA {
    auto sensor_state(
        PubSubClient& mqtt_client,
        SensorsState state
    ) -> void;
}  // namespace AA