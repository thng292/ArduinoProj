#include "utils.hpp"

#include <WiFiUdp.h>

#include "mqtt_action.hpp"

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
        .food = food_scale.get_units() / 2100.0f * 5000,
        .water = water_scale.get_units() / 2100.0f * 5000,
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
            Serial.println("Resubcribing");
            const auto* list =
                (const std::string_view*)&AA::MQTT_ACTION::TOPICS;
            for (uint8_t i = 0; i < AA::MQTT_ACTION::NUM_SUBSCRIBE_TOPICS;
                 i++) {
                Serial.printf("Subcribing to %s", list[i].data());
                Serial.println();
                mqtt_client.subscribe(list[i].data());
            }
        } else {
            Serial.println("Connect to MQTT broker failed. Retrying...");
            delay(5000);
        }
    }
}
