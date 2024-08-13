#pragma once

class WaterLevelSensor {
    float level = 0;

   public:
    auto read() -> float
    {
        auto res = level;
        level -= 5;
        return res;
    }
};