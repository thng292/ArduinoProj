#include "feed_scheduler.hpp"

#include <algorithm>
#include <charconv>

#include "mqtt_action.hpp"
#include "speaker.hpp"

auto AA::FeedScheduler::begin() -> void
{
    this->last_water_scale_value = this->water_scale->get_units();
}

auto AA::FeedScheduler::checkAndAddWater() -> void
{
    auto water_level = this->water_level_sensor->read();
    if (this->adding_water) {
        if (water_level >= hi_threshold) {
            this->water_servo->write(0);
            this->adding_water = false;
            auto new_water_scale_value = this->water_scale->get_units();
            constexpr uint32_t kg_to_gram = 1000;
            this->onAddWater(
                (new_water_scale_value - this->last_water_scale_value) *
                kg_to_gram
            );
            MQTT_ACTION::push_log(*(this->mqtt_client), "Done adding water!");
        }
    } else {
        if (water_level <= lo_threshold) {
            this->water_servo->write(30);
            MQTT_ACTION::push_log(*(this->mqtt_client), "Adding water!");
            this->adding_water = true;
        } else {
            this->last_water_scale_value = this->water_scale->get_units();
        }
    }
}

auto AA::FeedScheduler::checkAndAddFood() -> void
{
    if (this->adding_food) {
        auto food_weight = this->food_scale->get_units();
        if (food_weight - last_food_scale_value <= amount_to_feed_kg) {
            this->food_servo->write(0);
            this->adding_food = false;
            this->amount_to_feed_kg = 0;
        }
    } else {
        if (this->amount_to_feed_kg != 0) {
            this->food_servo->write(30);
            this->adding_food = true;
        } else {
            this->last_food_scale_value = this->food_scale->get_units();
        }
    }
}

auto AA::FeedScheduler::feed(float amount_kg) -> void
{
    this->amount_to_feed_kg += amount_kg;
}

auto AA::FeedScheduler::loop() -> void
{
    checkAndAddWater();
    checkAndAddFood();
    auto now = Time{
        .hour = this->ntp_client->getHours(),
        .minute = this->ntp_client->getMinutes(),
    };
    if (now.hour == this->endEating.hour and
        now.minute == this->endEating.minute) {
        this->onEndEating(endEating);
        endEating.hour = -1;
        endEating.minute = -1;
    }
    constexpr auto kg_to_gram = 1000;
    if (this->endEating.hour != -1) {
        for (int i = 0; i < this->schedules.size(); i++) {
            auto& sched = this->schedules[i];
            if (sched.time.hour == now.hour and
                sched.time.minute == now.minute) {
                if (not this->done_flag[i]) {
                    this->done_flag[i] = true;
                    this->feed((float)sched.amount_gram / kg_to_gram);
                    MQTT_ACTION::push_log(*(this->mqtt_client), "Feeding pet!");
                    this->onStartEating(now);
                    auto min =
                        this->ntp_client->getMinutes() + sched.duration_m;
                    auto hour = this->ntp_client->getHours() + min / 60;
                    min = min % 60;
                    if (this->endEating.hour < hour) {
                        this->endEating = {
                            .hour = hour,
                            .minute = min,
                        };
                    } else if (this->endEating.hour == hour) {
                        this->endEating.minute =
                            std::max(min, this->endEating.minute);
                    }
                    Speaker.play();
                }
            }
            this->done_flag[i] = false;
        }
    }
}

auto AA::FeedScheduler::shouldBeEating() const noexcept -> bool
{
    return (this->endEating.hour != 0 and this->endEating.minute != 0);
}

auto AA::FeedScheduler::parseAndWriteSchedule(const std::string& data) -> void
{
    constexpr auto assign =
        [](Schedule& sched, uint8_t stage, const char* first, const char* last
        ) -> boolean {
        auto res = std::from_chars_result{};
        switch (stage) {
            case 0:
                res = std::from_chars(first, last, sched.time.hour);
                break;
            case 1:
                res = std::from_chars(first, last, sched.time.minute);
                break;
            case 2:
                sched.audio_url = std::string(first, last);
                res.ptr = last;
                break;
            case 3:
                res = std::from_chars(first, last, sched.amount_gram);
                break;
            case 4:
                res = std::from_chars(first, last, sched.duration_m);
                break;
            default:
                res.ptr = first;
                break;
        }

        return res.ptr == last;
    };

    bool open_brack = false;
    size_t start_i = 0;
    uint8_t stage = 5;

    auto tmp = Schedule{};
    auto tmp_schedues = std::vector<Schedule>();

    for (int i = 0; i < data.size(); i++) {
        switch (data[i]) {
            case '{':
                if (open_brack or stage != 5) {
                    return;
                }
                open_brack = true;
                start_i = ++i;
                stage = 0;
                break;
            case '}':
                if (open_brack and stage == 4) {
                    assign(tmp, stage, data.data() + start_i, data.data() + i);
                    start_i = i + 1;
                    stage += 1;
                    tmp_schedues.push_back(tmp);
                    open_brack = false;
                } else {
                    return;
                }
                break;
            case ' ':
                if (not open_brack) {
                    return;
                }
                assign(tmp, stage, data.data() + start_i, data.data() + i);
                start_i = i + 1;
                stage += 1;
                break;
        }
    }

    this->schedules = tmp_schedues;
    // Serial.println(tmp_schedues.size());
    // for (const auto& sched : tmp_schedues) {
    //     Serial.println(sched.time.hour);
    //     Serial.println(sched.time.minute);
    //     Serial.println(sched.amount_gram);
    //     Serial.println(sched.duration_m);
    //     Serial.println(sched.audio_url.c_str());
    //     Serial.println();
    // }
}
