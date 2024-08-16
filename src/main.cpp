#include <DHT.h>
#include <ESP32Servo.h>
#include <Esp.h>
#include <HTTPClient.h>
#include <HX711.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include <string_view>

#include "component/button.hpp"
#include "component/camera.hpp"
#include "component/feed_scheduler.hpp"
#include "component/mic.hpp"
#include "component/pin_map.hpp"
#include "component/speaker.hpp"
#include "component/water_level_sensor.hpp"
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
auto ntp_client = AA::constructNTPClient();

auto butt = AA::Button(AA::Pin::BUTTON);
auto dht = DHT(AA::Pin::DHT22, DHT22);
auto food_scale = HX711();
auto water_scale = HX711();
auto food_servo = Servo();
auto water_servo = Servo();
auto water_level_sens = AA::WaterLevelSensor(AA::Pin::WATER_LEVEL_SENSOR);
auto camera = AA::Camera(AA::Pin::DISTANCE_SENS);
auto mic = AA::Mic(AA::Pin::MIC);

auto feed_scheduler = AA::FeedScheduler(
    &ntp_client,
    &food_servo,
    &food_scale,
    &water_servo,
    &water_scale,
    &water_level_sens
);

unsigned long start_eat_time = 0;

auto setup() -> void
{
    Serial.begin(9600);
    AA::connectWifi(AA::WIFI_SSID, AA::WIFI_PSWD);
    mqtt_client.setServer(AA::MQTT_BROKER_ADDR.data(), AA::MQTT_BROKER_PORT);
    ntp_client.begin();

    dht.begin();
    food_scale.begin(AA::Pin::SCALE[0].DT, AA::Pin::SCALE[0].SCK);
    water_scale.begin(AA::Pin::SCALE[1].DT, AA::Pin::SCALE[1].SCK);
    food_servo.attach(AA::Pin::SERVO[0]);
    water_servo.attach(AA::Pin::SERVO[1]);

    butt.begin();
    mic.begin();
    camera.begin();
    Speaker.attach(AA::Pin::BUZZER);
    feed_scheduler.begin();
    feed_scheduler.onAddWater = [](float water_added_gram) {
        AA::MQTT_ACTION::water_add(mqtt_client, water_added_gram);
    };
    feed_scheduler.onStartEating = [](auto _) {
        start_eat_time = millis();
    };
    feed_scheduler.onEndEating = [](auto _) {
        AA::MQTT_ACTION::time_eat(mqtt_client, millis() - start_eat_time);
    };

    pinMode(AA::Pin::BUILTIN_LED, OUTPUT);
    pinMode(AA::Pin::BUZZER, OUTPUT);
    for (auto pin : AA::Pin::SERVO) {
        pinMode(pin, INPUT);
    }

    AA::MQTT_ACTION::push_log(mqtt_client, LOG_PREFIX "Everything is ok");
}

auto loop() -> void
{
    if (not mqtt_client.loop()) {
        Serial.println(LOG_PREFIX "mqtt_client.loop() return false");
    }
    camera.loop();
    Speaker.loop();
    feed_scheduler.loop();
}
