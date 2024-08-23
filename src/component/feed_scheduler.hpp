#pragma once
#include <ESP32Servo.h>
#include <HX711.h>
#include <NTPClient.h>
#include <PubSubClient.h>

#include <functional>
#include <vector>
#include <mutex>

#include "water_level_sensor.hpp"

namespace AA {

    class FeedScheduler {
        struct Time {
            int hour = 0;
            int minute = 0;
        };

        struct Schedule {
            Time time;
            int amount_gram = 0;
            int duration_m = 0;
            std::string audio_url;
        };

        NTPClient* ntp_client = nullptr;
        Servo* food_servo;
        Servo* water_servo;
        HX711* food_scale;
        HX711* water_scale;
        WaterLevelSensor* water_level_sensor;
        PubSubClient* mqtt_client;

        std::vector<Schedule> schedules;
        std::mutex schedules_mutex;
        std::vector<bool> done_flag;

        bool adding_water = false;
        bool adding_food = false;
        float amount_to_feed_kg = 0;
        float last_water_scale_value = 0;
        float last_food_scale_value = 0;
        Time endEating = {-1, -1};

        constexpr static float hi_threshold = 0.8;
        constexpr static float lo_threshold = 0.5;

        auto checkAndAddWater() -> void;
        auto checkAndAddFood() -> void;
        auto getWaterWeight() -> float;
        auto getFoodWeight() -> float;

       public:
        // callback: auto cb(float water_added_gram) -> void
        std::function<void(float)> onAddWater;

        // callback: auto cb(FeedScheduler::Time current_time) -> void
        std::function<void(Time)> onStartEating;
        std::function<void(Time)> onEndEating;

        FeedScheduler(
            NTPClient* ntp_client,
            Servo* food_servo,
            HX711* food_scale,
            Servo* water_servo,
            HX711* water_scale,
            WaterLevelSensor* water_level_sensor,
            PubSubClient* mqtt_client
        )
            : ntp_client(ntp_client),
              food_servo(food_servo),
              food_scale(food_scale),
              water_servo(water_servo),
              water_scale(water_scale),
              water_level_sensor(water_level_sensor),
              mqtt_client(mqtt_client)
        {
        }

        auto begin() -> void;

        auto loop() -> void;

        auto feed(float amount_kg) -> void;

        [[nodiscard]] auto shouldBeEating() const noexcept -> bool;

        auto parseAndWriteSchedule(const std::string& data) -> void;
    };
};  // namespace AA