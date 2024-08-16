#pragma once
#include <Esp.h>
#include <NTPClient.h>
#include <WiFi.h>

#include <string_view>

#include "DHT.h"
#include "HX711.h"
#include "PubSubClient.h"
#include "component/water_level_sensor.hpp"

#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING STRINGIZE(__LINE__)

#define LOG_PREFIX __FILE__ ":" LINE_STRING ", "

namespace AA {
    struct SensorsState {
        float temp = 0;
        float humid = 0;
        float food = 0;
        float water = 0;
        float bowl_water_level = 0;
    };

    // Only call after get wifi
    auto constructNTPClient() -> NTPClient;

    auto updateSensorsState(
        DHT& dht,
        HX711& food_scale,
        HX711& water_scale,
        WaterLevelSensor& water_level_sens
    ) -> SensorsState;

    auto connectWifi(std::string_view WIFI_SSID, std::string_view WIFI_PSWD)
        -> void;

    auto mqtt_reconnect(PubSubClient& mqtt_client, std::string_view DEV_ID)
        -> void;
}  // namespace AA
