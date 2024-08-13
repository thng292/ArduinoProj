#include "utils.hpp"

#include <WiFiUdp.h>

auto AA::syncWithNTPServer() -> NTPClient
{
    auto wifi_udp = WiFiUDP();
    auto ntp_client = NTPClient(wifi_udp);
    ntp_client.begin();
    return ntp_client;
}

auto AA::updateSensorsState(
    DHT& dht,
    HX711& food_scale,
    HX711& water_scale,
    WaterLevelSensor& water_level_sens
) -> AA::SensorsState
{
    return SensorsState{
        .temp = dht.readTemperature(),
        .humid = dht.readHumidity(),
        .food = food_scale.get_units(),
        .water = water_scale.get_units(),
        .bowl_water_level = water_level_sens.read(),
    };
}
