#include <Esp.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include <string_view>

#include "component/button.hpp"
#include "component/pin_map.hpp"

// #include "song_embed.hpp"
namespace AA {
    using namespace std::string_view_literals;
    constexpr auto WIFI_SSID = "Wokwi-GUEST"sv;
    constexpr auto WIFI_PSWD = ""sv;
    // constexpr auto MQTT_BROKER_ADDR = "test.mosquitto.org"sv;
    constexpr auto MQTT_BROKER_ADDR = "192.168.1.9"sv;
    constexpr uint16_t MQTT_BROKER_PORT = 1883;
    constexpr auto DEV_ID = "idkwhattosethere"sv;

};  // namespace AA

auto wifi_client = WiFiClient();
auto mqtt_client = PubSubClient(wifi_client);
auto http_client = HTTPClient();

auto butt = AA::Button(AA::Pin::BUTTON);

auto connectWifi() -> void
{
    Serial.println("Connecting to Wifi");
    WiFi.begin(AA::WIFI_SSID.data(), AA::WIFI_PSWD.data());
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

auto mqtt_reconnect() -> void
{
    while (not mqtt_client.connected()) {
        Serial.println("Connecting to MQTT broker");
        if (mqtt_client.connect(AA::DEV_ID.data())) {
            Serial.println("Connected to MQTT broker");
        } else {
            Serial.println("Connect to MQTT broker failed. Retrying...");
            delay(5000);
        }
    }
}

auto setup() -> void
{
    Serial.begin(9600);
    connectWifi();
    mqtt_client.setServer(AA::MQTT_BROKER_ADDR.data(), AA::MQTT_BROKER_PORT);
    pinMode(AA::Pin::BUTTON, INPUT);
    pinMode(AA::Pin::BUILTIN_LED, OUTPUT);
    // AA::ReadWaveFile(song_embed);
}

auto loop() -> void
{
    digitalWrite(AA::Pin::BUILTIN_LED, butt.read());
    mqtt_reconnect();
    mqtt_client.loop();
    int temp = random(0, 100);
    char buffer[50] = {0};
    sprintf(buffer, "%d", temp);
    if (mqtt_client.publish("temperature", buffer)) {
        Serial.printf("Sent: %s\n", buffer);
    } else {
        Serial.println("Send failed");
    }
    delay(5000);
}
