#include "feed_scheduler.hpp"

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
            this->on_add_water(
                (new_water_scale_value - this->last_water_scale_value) *
                kg_to_gram
            );
        }
    } else {
        if (water_level <= lo_threshold) {
            this->water_servo->write(30);
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
    auto now = Time
    {
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
                    // FIXME: Play audio here
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
