#pragma once
#include <NTPClient.h>

#include "DHT.h"
#include "HX711.h"
#include "component/water_level_sensor.hpp"

namespace AA {
    struct SensorsState {
        float temp = 0;
        float humid = 0;
        float food = 0;
        float water = 0;
        float bowl_water_level = 0;
    };

    // Only call after get wifi
    auto syncWithNTPServer() -> NTPClient;

    auto updateSensorsState(
        DHT& dht,
        HX711& food_scale,
        HX711& water_scale,
        WaterLevelSensor& water_level_sens
    ) -> SensorsState;
}  // namespace AA
