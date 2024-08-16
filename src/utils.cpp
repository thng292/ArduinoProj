#include "utils.hpp"

#include <WiFiUdp.h>

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

auto AA::connectWifi(std::string_view WIFI_SSID, std::string_view WIFI_PSWD)
    -> void
{
    Serial.println("Connecting to Wifi");
    WiFi.begin(WIFI_SSID.data(), WIFI_PSWD.data());
    auto wifi_status = WiFi.status();
    while (wifi_status != WL_CONNECTED) {
        switch (wifi_status) {
            case WL_CONNECT_FAILED:
                Serial.println("\nConnect failed. Restarting...");
                ESP.restart();
                break;
            case WL_CONNECTION_LOST:
                Serial.println("\nConnect lost. Restarting...");
                ESP.restart();
                break;
            default:
                Serial.print('.');
                delay(500);
                break;
        }
        wifi_status = WiFi.status();
    }
    Serial.println();
    Serial.println("Wifi Connected");
}

auto AA::mqtt_reconnect(PubSubClient& mqtt_client, std::string_view DEV_ID)
    -> void
{
    while (not mqtt_client.connected()) {
        Serial.println("Connecting to MQTT broker");
        if (mqtt_client.connect(DEV_ID.data())) {
            Serial.println("Connected to MQTT broker");
        } else {
            Serial.println("Connect to MQTT broker failed. Retrying...");
            delay(5000);
        }
    }
}
