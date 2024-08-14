#include <ESP32Servo.h>
#include <Esp.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include <string_view>

#include "DHT.h"
#include "HX711.h"
#include "component/button.hpp"
#include "component/camera.hpp"
#include "component/mic.hpp"
#include "component/pin_map.hpp"
#include "component/water_level_sensor.hpp"
#include "esp32-hal-gpio.h"
#include "mqtt_action.hpp"
#include "utils.hpp"

namespace AA {
    using namespace std::string_view_literals;
    constexpr auto WIFI_SSID = "Wokwi-GUEST"sv;
    constexpr auto WIFI_PSWD = ""sv;
    constexpr auto MQTT_BROKER_ADDR = "192.168.1.9"sv;
    constexpr uint16_t MQTT_BROKER_PORT = 1883;
    constexpr auto DEV_ID = "idkwhattosethere"sv;

};  // namespace AA

auto wifi_client = WiFiClient();
auto mqtt_client = PubSubClient(wifi_client);
auto http_client = HTTPClient();

auto butt = AA::Button(AA::Pin::BUTTON);
auto dht = DHT(AA::Pin::DHT22, DHT22);
auto food_scale = HX711();
auto water_scale = HX711();
auto food_servo = Servo();
auto water_servo = Servo();
auto water_level_sens = WaterLevelSensor();
auto camera = AA::Camera(AA::Pin::DISTANCE_SENS);
auto mic = AA::Mic(AA::Pin::MIC);

auto setup() -> void
{
    Serial.begin(9600);
    AA::connectWifi(AA::WIFI_SSID, AA::WIFI_PSWD);
    mqtt_client.setServer(AA::MQTT_BROKER_ADDR.data(), AA::MQTT_BROKER_PORT);

    dht.begin();
    food_scale.begin(AA::Pin::SCALE[0].DT, AA::Pin::SCALE[0].SCK);
    water_scale.begin(AA::Pin::SCALE[1].DT, AA::Pin::SCALE[1].SCK);
    food_servo.attach(AA::Pin::SERVO[0]);
    water_servo.attach(AA::Pin::SERVO[1]);

    butt.begin();
    mic.begin();
    camera.begin();

    pinMode(AA::Pin::BUILTIN_LED, OUTPUT);
    pinMode(AA::Pin::BUZZER, OUTPUT);
    for (auto pin : AA::Pin::SERVO) {
        pinMode(pin, INPUT);
    }

    AA::MQTT_ACTION::push_log(mqtt_client, "Setup: Everything is ok");
}

auto loop() -> void
{
    if (butt.isPressed()) {
        Serial.println("Button Pressed");
        AA::mqtt_reconnect(mqtt_client, AA::DEV_ID);
        mqtt_client.loop();
        camera.loop();
        auto sens_state = AA::updateSensorsState(
            dht, food_scale, water_scale, water_level_sens
        );
        AA::MQTT_ACTION::sensor_state(mqtt_client, sens_state);
        if (camera.isPetIn()) {
            AA::MQTT_ACTION::add_image(mqtt_client, camera.captureAndUpload());
            AA::MQTT_ACTION::add_video(mqtt_client, camera.recordAndUpload());
            AA::MQTT_ACTION::time_eat(mqtt_client, 1234566);
        }
        AA::MQTT_ACTION::dev_info(mqtt_client);
        AA::MQTT_ACTION::push_log(mqtt_client, "Test OK!");
    }
}
